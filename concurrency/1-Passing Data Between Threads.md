## The promise - future communication channel

The methods for passing data to a thread we have discussed so far are both useful during thread construction: We can either pass arguments to the thread function using variadic templates or we can use a Lambda to capture arguments by value or by reference. The following example illustrates the use of these methods again:

![[Pasted image 20230924133644.png]]
A drawback of these two approaches is that the information flows from the parent thread (`main`) to the worker threads (`t1` and `t2`). In this section, we want to look at a way to pass data in the opposite direction - that is from the worker threads back to the parent thread.

In order to achieve this, the threads need to adhere to a strict synchronization protocol. There is such a mechanism available in the C++ standard that we can use for this purpose. This mechanism acts as a single-use channel between the threads. The sending end of the channel is called "promise" while the receiving end is called "future".

In the C++ standard, the class template std::promise provides a convenient way to store a value or an exception that will acquired asynchronously at a later time via a `std::future` object. Each `std::promise` object **is meant to be used only a single time and only a single direction**.

In the following example, we want to declare a promise which allows for transmitting a string between two threads and modifying it in the process.

![[Pasted image 20230924133825.png]]

After defining a message, we have to create a suitable promise that can take a string object. To obtain the corresponding future, we need to call the method `get_future()` on the promise. Promise and future are the two types of the communication channel we want to use to pass a string between threads. The communication channel set up in this manner can only pass a string.

We can now create a thread that takes a function and we will pass it the promise as an argument as well as the message to be modified. Promises can not be copied, because the promise-future concept is a two-point communication channel for one-time use. Therefore, we must pass the promise to the thread function using `std::move`. The thread will then, during its execution, use the promise to pass back the modified message.

The thread function takes the promise as an rvalue reference in accordance with move semantics. After waiting for several seconds, the message is modified and the method `set_value()` is called on the promise.

Back in the main thread, after starting the thread, the original message is printed to the console. Then, we start listening on the other end of the communication channel by calling the function `get()` on the future. This method will block until data is available - which happens as soon as set_value has been called on the promise (from the thread). If the result is movable (which is the case for `std::string`), it will be moved - otherwise it will be copied instead. After the data has been received (with a considerable delay), the modified message is printed to the console.

![[Pasted image 20230924134213.png]]

It is also possible that the worker value calls set_value on the promise before `get()` is called on the future. In this case, `get()` returns immediately without any delay. After `get()` has been called once, the future is no longer usable. This makes sense as the normal mode of data exchange between promise and future works with `std::move` - and in this case, the data is no longer available in the channel after the first call to `get()`. If `get()` is called a second time, an exception is thrown.

#### get() vs. wait()

There are some situations where it might be interesting to separate the waiting for the content from the actual retrieving. Futures allow us to do that using the `wait()` function. This method will block until the future is ready. Once it returns, it is guaranteed that data is available and we can use `get()` to retrieve it without delay.

In addition to wait, the C++ standard also offers the method `wait_for`, which takes a time duration as an input and also waits for a result to become available. The method `wait_for()` will block either until the specified timeout duration has elapsed or the result becomes available - whichever comes first. The return value identifies the state of the result.

In the following example, please use the wait_for method to wait for the availability of a result for one second. After the time has passed (or the result is available) print the result to the console. Should the time be up without the result being available, print an error message to the console instead.

![[Pasted image 20230924134509.png]]

#### Passing exceptions[](https://qzahvsjf35.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Passing-exceptions)

The future-promise communication channel may also be used for passing exceptions. To do this, the worker thread simply sets an exception rather than a value in the promise. In the parent thread, the exception is then re-thrown once `get()` is called on the future.

Let us take a look at the following example to see how this mechanism works:

![[Pasted image 20230924134652.png]]

In the thread function, we need to implement a try-catch block which can be set to catch a particular exception or - as in our case - to catch all exceptions. Instead of setting a value, we now want to throw a `std::exception` along with a customized error message. In the catch-block, we catch this exception and throw it to the parent thread using the promise with set_exception. The function `std::current_exception` allows us to easily retrieve the exception which has been thrown.

On the parent side, we now need to catch this exception. In order to do this, we can use a try-block around the call to `get()`. We can set the catch-block to catch all exceptions or - as in this example - we could also catch a particular one such as the standard exception. Calling the method `what()` on the exception allows us to retrieve the message from the exception - which is the one defined on the promise side of the communication channel.

When we run the program, we can see that the exception is being thrown in the worker thread with the main thread printing the corresponding error message to the console.

So a promise future pair can be used to pass either values or exceptions between threads.

## Thread vs Tasks

#### Starting threads with async[](https://yhn6bqd4dw.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Starting-threads-with-async)

In the last section we have seen how data can be passed from a worker thread to the parent thread using promises and futures. A disadvantage of the promise-future approach however is that it is very cumbersome (and involves a lot of boilerplate code) to pass the promise to the thread function using an rvalue reference and `std::move`. For the straight-forward task of returning data or exceptions from a worker thread to the parent thread however, there is a simpler and more convenient way using `std::async()` instead of `std::thread()`.

Let us adapt the code example from the last section to use `std::async`:

![[Pasted image 20230924135133.png]]

The first change we are making is in the thread function: We are removing the promise from the argument list as well as the try-catch block. Also, the return type of the function is changed from void to double as the result of the computation will be channeled back to the main thread using a simple return. After these changes, the function has no knowledge of threads, nor of futures or promises - it is a simple function that takes two doubles as arguments and returns a double as a result. Also, it will throw an exception when a division by zero is attempted.

In the main thread, we need to replace the call to `std::thread` with `std::async`. Note that async returns a future, which we will use later in the code to retrieve the value that is returned by the function. A promise, as with `std::thread`, is no longer needed, so the code becomes much shorter. In the try-catch block, nothing has changed - we are still calling `get()` on the future in the try-block and exception-handling happens unaltered in the catch-block. Also, we do not need to call `join()` any more. With `async`, the thread destructor will be called automatically - which reduces the risk of a concurrency bug.

When we execute the code in the previous example, the output is identical to before, so we seemingly have the same functionality as before - or do we? When we use the `std::this_thread::get_id()` to print the system thread ids of the main and of the worker thread, we get the following command line output:

![[Pasted image 20230924135609.png]]

As expected, the ids between the two threads differ from each other - they are running in parallel. However, one of the major differences between `std::thread` and `std::async` is that with the latter, the system decides wether the associated function should be run asynchronously or synchronously. By adjusting the launch parameters of `std::async` manually, we can directly influence wether the associated thread function will be executed synchronously or asynchronously.

The line

![[Pasted image 20230924135619.png]]

enforces the synchronous execution of `divideByNumber`, which results in the following output, where the thread ids for main and worker thread are identical.

![[Pasted image 20230924135624.png]]

If we were to use the launch option "async" instead of "deferred", we would enforce an asynchronous execution whereas the option "any" would leave it to the system to decide - which is the default.

At this point, let us compare `std::thread` with `std::async`: Internally, `std::async` creates a promise, gets a future from it and runs a template function that takes the promise, calls our function and then either sets the value or the exception of that promise - depending on function behavior. The code used internally by `std::async` is more or less identical to the code we used in the previous example, except that this time it has been generated by the compiler and it is hidden from us - which means that the code we write appears much cleaner and leaner. Also, `std::async` makes it possible to control the amount of concurrency by passing an optional launch parameter, which enforces either synchronous or asynchronous behavior. This ability, especially when left to the system, allows us to prevent an overload of threads, which would eventually slow down the system as threads consume resources for both management and communication. If we were to use too many threads, the increased resource consumption would outweigh the advantages of parallelism and slow down the program. By leaving the decision to the system, we can ensure that the number of threads is chosen in a carefully balanced

## Task-based concurrency[](https://yhn6bqd4dw.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Task-based-concurrency)

Determining the optimal number of threads to use is a hard problem. It usually depends on the number of available cores wether it makes sense to execute code as a thread or in a sequential manner. The use of `std::async` (and thus tasks) take the burden of this decision away from the user and let the system decide wether to execute the code sequentially or as a thread. With tasks, the programmer decides what CAN be run in parallel in principle and the system then decides at runtime what WILL be run in parallel.

Internally, this is achieved by using thread-pools wich represent the number of available threads based on the cores/processors as well as by using work-stealing queues, where tasks are re-distributed among the available processors dynamically. The following diagram shows the principal of task distribution on a multi-core system using work stealing queues.

![[Pasted image 20231025124319.png]]

As can be seen, the first core in the example is heavily oversubscribed with several tasks that are waiting to be executed. The other cores however are running idle. The idea of a work-stealing queue is to have a watchdog program running in the background that regularly monitors the amount of work performed by each processor and redistributes it as needed. For the above example this would mean that tasks waiting for execution on the first core would be shifted (or "stolen") from busy cores and added to available free cores such that idle time is reduced. After this rearranging procedire, the task distribution in our example could look as shown in the following diagram.

![[Pasted image 20231025124310.png]]
A work distribution in this manner can only work, when parallelism is explicitly described in the program by the programmer. If this is not the case, work-stealing will not perform effectively.

To conclude this section, a general comparison of task-based and thread-based programming is given in the following:

With tasks, the system takes care of many details (e.g. join). With threads, the programmer is responsible for many details. As far as resources go, threads are usually more heavy-weight as they are generated by the operating system (OS). It takes time for the OS to be called and to allocate memory / stack / kernel data structures for the thread. Also, destroying the thread is expensive. Tasks on the other hand are more light-weight as they will be using a pool of already created threads (the "thread pool").

Threads and tasks are used for different problems. Threads have more to do with latency. When you have functions that can block (e.g. file input, server connection), threads can avoid the program to be blocked, when e.g. the server is waiting for a response. Tasks on the other hand focus on throughput, where many operations are executed in parallel.

## Assessing the advantage of parallel execution[](https://yhn6bqd4dw.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Assessing-the-advantage-of-parallel-execution)

In this section, we want to explore the influence of the number of threads on the performance of a program with respect to its overall runtime. The example below has a thread function called "workerThread" which contains a loop with an adjustable number of cycles in which a mathematical operation is performed.
![[Pasted image 20231025125529.png]]
In `main()`, a for-loop starts a configurable number of tasks that can either be executed synchronously or asynchronously. As an experiment, we will now use a number of different parameter settings to execute the program and evaluate the time it takes to finish the computations. The idea is to gauge the effect of the number of threads on the overall runtime:

1. int nLoops = 1e7 , nThreads = 4 , std::launch::async
    ![[Pasted image 20231025125521.png]]
    
    With this set of parameters, the high workload is computed in parallel, with an overall runtime of ~45 milliseconds.
    
2. int nLoops = 1e7 , nThreads = 5 , std::launch::deferred
   ![[Pasted image 20231025125516.png]]
    
    The difference to the first set of parameters is the synchronous execution of the tasks - all computations are performed sequentially - with an overall runtime of ~126 milliseconds. While impressive with regard to the achieved speed-up, the relative runtime advantage of setting 1 to this settings is at a factor of ~2.8 on a 4-core machine.
    
3. int nLoops = 10 , nThreads = 5 , std::launch::async
    ![[Pasted image 20231025125510.png]]
    
    In this parameter setting, the tasks are run in parallel again but with a significantly lower number of computations: The thread function now computes only 10 square roots where with settings 1 and 2 a total of 10.000.000 square roots were computed. The overall runtime of this example therefore is significantly lower with only ~3 milliseconds.
    
4. int nLoops = 10 , nThreads = 5 , std::launch::deferred
    ![[Pasted image 20231025125502.png]]
    
    In this last example, the same 10 square roots are computed sequentially. Surprising, the overall runtime is at only 0.01 milliseconds - an astounding difference to the asynchronous execution and a stark reminder that starting and managing threads takes a significant amount of time. It is therefore not a general advantage if computations are performed in parallel: It must be carefully weighed with regard to the computational effort whether parallelization makes sense.

# Avoiding Data Races

## Understanding data races[](https://4vak6ykjwz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Understanding-data-races)

One of the primary sources of error in concurrent programming are data races. They occur, when two concurrent threads are accessing the same memory location while at least one of them is modifying (the other thread might be reading or modifying). In this scenario, the value at the memory location is completely undefined. Depending on the system scheduler, the second thread will be executed at an unknown point in time and thus see different data at the memory location with each execution. Depending on the type of program, the result might be anything from a crash to a security breach when data is read by a thread that was not meant to be read, such as a user password or other sensitive information. Such an error is called a „data race“ because two threads are racing to get access to a memory location first, with the content at the memory location depending on the result of the race.

The following diagram illustrates the principle: One thread wants to increment a variable `x`, whereas the other thread wants to print the same variable. Depending on the timing of the program and thus the order of execution, the printed result might change each time the program is executed.

![[Pasted image 20231025132034.png]]

In this example, one safe way of passing data to a thread would be to carefully synchronize the two threads using either `join()` or the promise-future concept that can guarantee the availability of a result. Data races are always to be avoided. Even if nothing bad seems to happen, they are a bug and should always be treated as such. Another possible solution for the above example would be to make a copy of the original argument and pass the copy to the thread, thereby preventing the data race.

## Passing data to a thread by value[](https://4vak6ykjwz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Passing-data-to-a-thread-by-value)

In the following example, an instance of the proprietary class `Vehicle` is created and passed to a thread by value, thus making a copy of it.
![[Pasted image 20231025132507.png]]

Note that the class Vehicle has a default constructor and an initializing constructor. In the main function, when the instances `v0` and `v1` are created, each constructor is called respectively. Note that `v0` is passed by value to a Lambda, which serves as the thread function for `std::async`. Within the Lambda, the id of the Vehicle object is changed from the default (which is 0) to a new value 2. Note that the thread execution is paused for 500 milliseconds to guarantee that the change is performed well after the main thread has proceeded with its execution.

In the `main` thread, immediately after starting up the worker thread, the id of `v0` is changed to 3. Then, after waiting for the completion of the thread, the vehicle id is printed to the console. In this program, the output will always be the following:
![[Pasted image 20231025132515.png]]

Passing data to a thread in this way is a clean and safe method as there is no danger of a data race - at least when atomic data types such as integers, doubles, chars or booleans are passed.

When passing a complex data structure however, there are sometimes pointer variables hidden within, that point to a (potentially) shared data buffer - which might cause a data race even though the programmer believes that the copied data will effectively preempt this. The next example illustrates this case by adding a new member variable to the `Vehicle` class, which is a pointer to a string object, as well as the corresponding getter and setter functions.
![[Pasted image 20231025132727.png]]

The output of the program looks like this:
![[Pasted image 20231025132735.png]]
The basic program structure is mostly identical to the previous example with the object `v0` being copied by value when passed to the thread function. This time however, even though a copy has been made, the original object `v0` is modified, when the thread function sets the new name. This happens because the member `_name` is a pointer to a string and after copying, even though the pointer variable has been duplicated, it still points to the same location as its value (i.e. the memory location) has not changed. Note that when the delay is removed in the thread function, the console output varies between "Vehicle 2" and "Vehicle 3", depending on the system scheduler. Such an error might go unnoticed for a long time. It could show itself well after a program has been shipped to the client - which is what makes this error type so treacherous.

Classes from the standard template library usually implement a deep copy behavior by default (such as `std::vector`). When dealing with proprietary data types, this is not guaranteed. The only safe way to tell whether a data structure can be safely passed is by looking at its implementation: Does it contain only atomic data types or are there pointers somewhere? If this is the case, does the data structure implement the copy constructor (and the assignment operator) correctly? Also, if the data structure under scrutiny contains sub-objects, their respective implementation has to be analyzed as well to ensure that deep copies are made everywhere.

Unfortunately, one of the primary concepts of object-oriented programming - information hiding - often prevents us from looking at the implementation details of a class - we can only see the interface, which does not tell us what we need to know to make sure that an object of the class may be safely passed by value.

## Overwriting the copy constructor[](https://4vak6ykjwz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Overwriting-the-copy-constructor)

The problem with passing a proprietary class is that the standard copy constructor makes a 1:1 copy of all data members, including pointers to objects. This behavior is also referred to as "shallow copy". In the above example we would have liked (and maybe expected) a "deep copy" of the object though, i.e. a copy of the data to which the pointer refers. A solution to this problem is to create a proprietary copy constructor in the class `Vehicle`. The following piece of code overwrites the default copy constructor and can be modified to make a customized copy of the data members.
![[Pasted image 20231025135526.png]]

Expanding on the code example from above, please implement the code required for a deep copy so that the program always prints "Vehicle 3" to the console, regardless of the delay within the thread function.

## Passing data using move semantics[](https://4vak6ykjwz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Passing-data-using-move-semantics)

Even though a customized copy constructor can help us to avoid data races, it is also time (and memory) consuming. In the following, we will use move semantics to implement a more effective way of safely passing data to a thread.

A move constructor enables the resources owned by an rvalue object to be moved into an lvalue without physically copying it. Rvalue references support the implementation of move semantics, which enables the programmer to write code that transfers resources (such as dynamically allocated memory) from one object to another.

To make use of move semantics, we need to provide a move constructor (and optionally a move assignment operator). Copy and assignment operations whose sources are rvalues automatically take advantage of move semantics. Unlike the default copy constructor however, the compiler does not provide a default move constructor.

To define a move constructor for a C++ class, the following steps are required:

1. Define an empty constructor method that takes an rvalue reference to the class type as its parameter
    ![[Pasted image 20231025135645.png]]
    
2. In the move constructor, assign the class data members from the source object to the object that is being constructed
    ![[Pasted image 20231025135700.png]]
    
3. Assign the data members of the source object to default values.
    ![[Pasted image 20231025135730.png]]
    

When launching the thread, the Vehicle object `v0` can be passed using `std::move()` - which calls the move constructor and invalidates the original object `v0` in the main thread.

## Move semantics and uniqueness[](https://4vak6ykjwz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Move-semantics-and-uniqueness)

As with the above-mentioned copy constructor, passing by value is usually safe - provided that a deep copy is made of all the data structures within the object that is to be passed. With move semantics , we can additionally use the notion of uniqueness to prevent data races by default. In the following example, a `unique_pointer` instead of a raw pointer is used for the string member in the Vehicle class.
![[Pasted image 20231025140000.png]]
As can be seen, the `std::string` has now been changed to a unique pointer, which means that only a single reference to the memory location it points to is allowed. Accordingly, the move constructor transfers the unique pointer to the worker by using `std::move` and thus invalidates the pointer in the `main` thread. When calling `v0.getName()`, an exception is thrown, making it clear to the programmer that accessing the data at this point is not permissible - which is the whole point of using a unique pointer here as a data race will now be effectively prevented.

The point of this example has been to illustrate that move semantics on its own is not enough to avoid data races. The key to thread safety is to use move semantics in conjunction with uniqueness. It is the responsibility of the programmer to ensure that pointers to objects that are moved between threads are unique.

Useful tip: using std sleep for can expose data races.