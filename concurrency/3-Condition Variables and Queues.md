# The Monitor Object Pattern[](https://jnfeue3rna.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-Monitor-Object-Pattern)

## The monitor object pattern[](https://jnfeue3rna.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-monitor-object-pattern)

In the previous sections we have learned that data protection is a critical element in concurrent programming. After looking at several ways to achieve this, we now want to build on these concepts to devise a method for a controlled and finely-grained data exchange between threads - a concurrent message queue. One important step towards such a construct is to implement a monitor object, which is a design pattern that synchronizes concurrent method execution to ensure that only one method at a time runs within an object. It also allows an object's methods to cooperatively schedule their execution sequences. The problem solved by this pattern is based on the observation that many applications contain objects whose methods are invoked concurrently by multiple client threads. These methods often modify the state of their objects, for example by adding data to an internal vector. For such concurrent programs to execute correctly, it is necessary to synchronize and schedule access to the objects very carefully. The idea of a monitor object is to synchronize the access to an object's methods so that only one method can execute at any one time.

In a previous section, we have looked at a code example which came pretty close to the functionality of a monitor object : the class `WaitingVehicles`.

Let us modify and partially reimplement this class, which we want to use as a shared place where concurrent threads may store data, in our case instances of the class `Vehicle`. As we will be using the same `WaitingVehicles` object for all the threads, we have to pass it to them by reference - and as all threads will be writing to this object at the same time (which is a mutating operation) we will pass it as a shared pointer. Keep in mind that there will be many threads that will try to pass data to the `WaitingVehicles` object simultaneously and thus there is the danger of a data race.

Before we take a look at the implementation of `WaitingVehicles`, let us look at the main function first where all the threads are spawned. We need a vector to store the futures as there is no data to be returned from the threads. Also, we need to call `wait()` on the futures at the end of `main()` so the program will not prematurely exit before the thread executions are complete.
![[Pasted image 20231026070507.png]]
Instead of using `push_back` we will again be using `emplace_back` to construct the futures in place rather than moving them into the vector. After constructing a new `Vehicle` object within the for-loop, we start a new task by passing it a reference to the `pushBack` function, a shared pointer to our `WaitingVehicles` object and the newly created vehicle. Note that the latter is passed using move semantics.

Now let us take a look at the implementation of the `WaitingVehicle` object.

![[Pasted image 20231026070523.png]]

We need to enable it to process write requests from several threads at the same time. Every time a request comes in from a thread, the object needs to add the new data to its internal storage. Our storage container will be an `std::vector`. As we need to protect the vector from simultaneous access later, we also need to integrate a mutex into the class. As we already know, a mutex has the methods lock and unlock. In order to avoid data races, the mutex needs to be locked every time a thread wants to access the vector and it needs to be unlocked once the write operation is complete. In order to avoid a program freeze due to a missing unlock operation, we will be using a lock guard object, which automatically unlocks once the lock object gets out of scope.

In our modified `pushBack` function, we will first create a lock guard object and pass it the mutex member variable. Now we can freely move the Vehicle object into our vector without the danger of a data race. At the end of the function, there is a call to `std::sleep_for`, which simulates some work and helps us to better expose potential concurrency problems. With each new Vehicle object that is passed into the queue, we will see an output to the console.

Another function within the `WaitingVehicle` class is `printIDs()`, which loops over all the elements of the vector and prints their respective IDs to the console. One major difference between `pushBack()` and `printIDs()` is that the latter function accesses all `Vehicle` objects by looping through the vector while `pushBack` only accesses a single object - which is the newest addition to the `Vehicle` vector.

When the program is executed, the following output is printed to the console:

![[C4-4-A2.gif]]

As can be seen, the `Vehicle` objects are added one at a time, with all threads duly waiting for their turn. Then, once all `Vehicle` objects have been stored, the call to `printIDs` prints the entire content of the vector all at once.

While the functionality of the monitor object we have constructed is an improvement over many other methods that allow passing data to threads, it has one significant disadvantage: The main thread has to wait until all worker threads have completed their jobs and only then can it access the added data in bulk. A system which is truly interactive however has to react to events as they arrive - it should not wait until all threads have completed their jobs but instead act immediately as soon as new data arrives. In the following, we want to add this functionality to our monitor object.

![[Pasted image 20231026071321.png]]

In the `main` thread, we will use an infinite while-loop to frequently poll the monitor object and check whether new data has become available. Contrary to before, we will now perform the read operation before the workers are done - so we have to integrate our loop before `wait()` is called on the futures at the end of `main()`. Once a new `Vehicle` object becomes available, we want to print it within the loop.

When we execute the code, we get a console output similar to the one listed below:

![[Pasted image 20231026071343.png]]

From the output it can easily be seen, that adding and removing to and from the monitor object is now interleaved. When executed repeatedly, the order of the vehicles will most probably differ between executions.

# Building a Concurrent Message Queue[](https://s33a7snfps.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Building-a-Concurrent-Message-Queue)

## Condition variables[](https://s33a7snfps.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Condition-variables)

The polling loop we have used in the previous example has not been programmed optimally: As long as the program is running, the while-loop will keep the processor busy, constantly asking wether new data is available. In the following, we will look at a better way to solve this problem without putting too much load on the processor.

The alternative to a polling loop is for the main thread to block and wait for a signal that new data is available. This would prevent the infinite loop from keeping the processor busy. We have already discussed a mechanism that would fulfill this purpose - the promise-future construct. The problem with futures is that they can only be used a single time. Once a future is ready and `get()` has been called, it can not be used any more. For our purpose, we need a signaling mechanism that can be re-used. The C++ standard offers such a construct in the form of "condition variables".

A `std::condition_variable` has a method `wait()`, which blocks, when it is called by a thread. The condition variable is kept blocked until it is released by another thread. The release works via the method `notify_one()` or the `notify_all` method. The key difference between the two methods is that notify_one will only wake up a single waiting thread while `notify_all` will wake up all the waiting threads at once.

A condition variable is a low-level building block for more advanced communication protocols. It neither has a memory of its own nor does it remember notifications. Imagine that one thread calls `wait()` before another thread calls `notify()`, the condition variable works as expected and the first thread will wake up. Imagine the case however where the call order is reversed such that `notify()` is called before `wait()`, the notification will be lost and the thread will block indefinitely. So in more sophisticated communication protocols a condition variable should always be used in conjunction with another shared state that can be checked independently. Notifying a condition variable in this case would then only mean to proceed and check this other shared state.

Let us pretend our shared variable was a boolean called `dataIsAvailable`. Now let’s discuss two scenarios for the protocol depending on who acts first, the producer or the consumer thread.

### Scenario 1:[](https://s33a7snfps.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Scenario-1:)
![[Pasted image 20231026074839.png]]
The consumer thread checks `dataIsAvailable()` and since it is false, the consumer thread blocks and waits on the condition variable. Later in time, the producer thread sets dataIsAvailable to true and calls `notify_one` on the condition variable. At this point, the consumer wakes up and proceeds with its work.

### Scenario 2:[](https://s33a7snfps.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Scenario-2:)
![[Pasted image 20231026074829.png]]
Here, the producer thread comes first, sets `dataIsAvailable()` to true and calls `notify_one`. Then, the consumer thread comes and checks `dataIsAvailable()` and finds it to be true - so it does not call wait and proceeds directly with its work. Even though the notification is lost, it does not cause a problem in this construct - the message has been passed successfully through dataIsAvailable and the wait-lock has been avoided.

In an ideal (non-concurrent) world, these two scenarios would most probably be sufficient to describe to possible combinations. But in concurrent programming, things are not so easy. As seen in the diagrams, there are four atomic operations, two for each thread. So when executed often enough, all possible interleavings will show themselves - and we have to find the ones that still cause a problem.

Here is one combination that will cause the program to lock:
![[Pasted image 20231026074820.png]]
The consumer thread reads `dataIsAvailable()`, which is false in the example. Then, the producer sets `dataIsAvailable()` to true and calls notify. Due to this unlucky interleaving of actions, the consumer thread calls wait because it has seen `dataIsAvailable()` as false. This is possible because the consumer thread tasks are not a joint atomic operation but may be separated by the scheduler and interleaved with some other tasks - in this case the two actions performed by the producer thread. The problem here is that after calling wait, the consumer thread will never wake up again. Also, as you may have noticed, the shared variable dataReady is not protected by a mutex here - which makes it even more likely that something will go wrong.

One quick idea for a solution which might come to mind would be to perform the two operations dataIsAvailable and wait under a locked mutex. While this would effectively prevent the interleaving of tasks between different threads, it would also prevent another thread from ever modifying dataIsAvailable again.

One reason for discussing these failed scenarios in such depth is to make you aware of the complexity of concurrent behavior - even with a simple protocol like the one we are discussing right now.

So let us now look at the final solution to the above problems and thus a working version of our communication protocol.
![[Pasted image 20231026074810.png]]
As seen above, we are closing the gap between reading the state and entering the wait. We are reading the state under the lock (red bar) and we call wait still under the lock. Then, we let wait release the lock and enter the wait state in one atomic step. This is only possible because the `wait()` method is able to take a lock as an argument. The lock that we can pass to wait however is not the `lock_guard` we have been using so often until now but instead it has to be a lock that can be temporarily unlocked inside wait - a suitable lock for this purpose would be the `unique_lock` type which we have discussed in the previous section.

## Implementing the WaitingVehicles queue[](https://s33a7snfps.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Implementing-the-WaitingVehicles-queue)

Now that we have all the ingredients to implement the concurrent queue to store waiting Vehicle objects, let us start with the implementation according to the diagram above.

1. The first step is to add a condition variable to `WaitingVehicles` class as a private member - just as the mutex.
![[Pasted image 20231026080624.png]]
2. The next step is to notify the client after pushing a new Vehicle into the vector.
![[Pasted image 20231026080617.png]]
3. In the method `popBack`, we need to create the lock first - it can not be a `lock_guard` any more as we need to pass it to the condition variable - to its method wait. Thus it must be a `unique_lock`. Now we can enter the wait state while at same time releasing the lock. It is only inside wait, that the mutex is temporarily unlocked - which is a very important point to remember: We are holding the lock before AND after our call to wait - which means that we are free to access whatever data is protected by the mutex. In our example, this will be `dataIsAvailable()`.
    
    Before we continue, we need to discuss the problem of "spurious wake-ups": Once in a while, the system will - for no obvious reason - wake up a thread. If such a spurious wake-up happened with taking proper precautions, we would issue wait without new data being available (because the wake-up has not been caused by the condition variable but by the system in this case). To prevent the call to wait in this case, we have to modify the code slightly:
    
![[Pasted image 20231026080608.png]]
3. (continued) In this code, even after a spurious wake-up, we are now checking wether data really is available. If so, we would be issuing the call to wait on the condition variable. And only if we are inside wait, may other threads modify and access `dataIsAvailable`.
    
    If the vector is empty, `wait` is called. When the thread wakes up again, the condition is immediately re-checked and - in case it has not been a spurious wake-up we can continue with our job and retrieve the vector.
    
    We can further simplify this code by letting the `wait()` function do the testing as well as the looping for us. Instead of the while loop, we can just pass a Lambda to `wait()`, which repeatedly checks wether the vector contains elements (thus the inverted logical expression):
    
![[Pasted image 20231026080558.png]]
3. (continued) When `wait()` finishes, we are guaranteed to find a new element in the vector this time. Also, we are still holding the lock and thus no other thread is able to access the vector - so there is no danger of a data race in this situation. As soon as we are out of scope, the lock will be automatically released.
    
4. In the main() function, there is still the polling loop that infinitely queries the availability of new Vehicle objects. But contrary to the example before, a call to popBack now puts the main() thread into a wait state and only resumes when new data is available - thus significantly reducing the load to the processor.
    

Let us now take a look at the complete code…
![[Pasted image 20231026080547.png]]
… and at the console output it produces:
![[Pasted image 20231026080539.png]]

## Exercise: Building a generic message queue[](https://s33a7snfps.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Exercise:-Building-a-generic-message-queue)

The code we have produced to manage waiting vehicles in our traffic simulation is in fact a very generic piece of code. Instead of passing Vehicle objects, it could very easily be modified to pass almost any kind of object or data type between two vector. So what we have created could be easily described as a generic message queue.

The following changes are required to turn the `WaitingVehicles` queue into a generic message queue:

1. Enable the class for templates by prepending `template<class T>` and change the name of the class to `MessageQueue`
2. Replace the `std::vector` by a `std::deque` as it makes more sense to retrieve the objects in the order "first-in, first-out (FIFO)". Also, rename it to `_messages`.
3. Adapt the method `pushBack` for use with templates and rename it `send`. Do the same with `popBack` and rename it to `receive`.
4. Test the queue by executing the following code in main:
![[Pasted image 20231026083137.png]]

LINK to generic message queue solution
![[example_generic_message_queue.cpp]]

```embed-cpp

```
```embed-cpp 
PATH: "vault://concurrency/code/4_condition_vars_and_queues/concurrent_message_queue/example_generic_message_queue.cpp"
TITLE: "EMBEDDED generic message queue solution"
```
