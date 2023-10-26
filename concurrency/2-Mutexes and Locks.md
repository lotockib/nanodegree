# Using a Mutex To Protect Shared Data[](https://svefnyqeq7.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Using-a-Mutex-To-Protect-Shared-Data)

## The mutex entity[](https://svefnyqeq7.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-mutex-entity)

Until now, the methods we have used to pass data between threads were short-term and involved passing an argument (the promise) from a parent thread to a worker thread and then passing a result back to the parent thread (via the future) once it has become available. The promise-future construct is a non-permanent communication channel for one-time usage.

We have seen that in order to avoid data races, we need to either forego accessing shared data or use it in read-only access without mutating the data. In this chapter, we want to look at a way to establish a stable long-term communication channel that allows for both sharing and mutation. Ideally, we would like to have a communication protocol that corresponds to voice communication over a radio channel, where the transmitter uses the expression "over" to indicate the end of the transmission to the receiver. By using such a protocol, sender and receiver can take turns in transmitting their data. In C++, this concept of taking turns can be constructed by an entity called a "mutex" - which stands for MUtual EXclusion.

Recall that a data race requires simultaneous access from two threads. If we can guarantee that only a single thread at a time can access a particular memory location, data races would not occur. In order for this to work, we would need to establish a communication protocol. It is important to note that a mutex is not the solution to the data race problem per se but merely an enabler for a thread-safe communication protocol that has to be implemented and adhered to by the programmer.
![[Pasted image 20231025185933.png]]
Let us take a look at how this protocol works: Assuming we have a piece of memory (e.g. a shared variable) that we want to protect from simultaneous access, we can assign a mutex to be the guardian of this particular memory. It is important to understand that a mutex is bound to the memory it protects. A thread 1 who wants to access the protected memory must "lock" the mutex first. After thread 1 is "under the lock", a thread 2 is blocked from access to the shared variable, it can not acquire the lock on the mutex and is temporarily suspended by the system.

Once the reading or writing operation of thread 1 is complete, it must "unlock" the mutex so that thread 2 can access the memory location. Often, the code which is executed "under the lock" is referred to as a "critical section". It is important to note that also read-only access to the shared memory has to lock the mutex to prevent a data race - which would happen when another thread, who might be under the lock at that time, were to modify the data.

When several threads were to try to acquire and lock the mutex, only one of them would be successful. All other threads would automatically be put on hold - just as cars waiting at an intersection for a green light (see the final project of this course). Once the thread who has succeeded in acquiring the lock had finished its job and unlocked the mutex, a queued thread waiting for access would be woken up and allowed to lock the mutex to proceed with his read / write operation. If all threads

## Exercise
![[Pasted image 20231025191144.png]]
This code builds on some of the classes we have seen in the previous lesson project - the concurrent traffic simulation. There is a class Vehicle that has a single data member (`int _id`). Also, there is a class WaitingVehicles, which is supposed to store a number of vehicles in an internal vector. Note that contrary to the lesson project, a vehicle is moved into the vector using an rvalue reference. Also note that the push_back function is commented out here. The reason for this is that we are trying to provoke a data race - leaving push_back active would cause the program to crash (we will comment it in later). This is also the reason why there is an auxiliary member `_tmpVehicles`, which will be used to count the number of Vehicles added via calls to `pushBack()`. This temporary variable will help us expose the data race without crashing the program.

In `main()`, a for-loop is used to launch a large number of tasks who all try to add a newly created Vehicle to the queue. Running the program synchronously with launch option `std::launch::deferred` generates the following output on the console:
![[Pasted image 20231025191159.png]]
Just as one would have expected, each task inserted an element into the queue with the total number of vehicles amounting to 1000.

Now let us enforce a concurrent behavior and change the launch option to `std::launch::async`. This generates the following output (with different results each time):
![[Pasted image 20231025191210.png]]
It seems that not all the vehicles could be added to the queue. But why is that? Note that in the thread function "pushBack" there is a call to `sleep_for`, which pauses the thread execution for a short time. This is the position where the data race occurs: First, the current value of `_tmpVehicles` is stored in a temporary variable `oldNum`. While the thread is paused, there might (and will) be changes to `_tmpVehicles` performed by other threads. When the execution resumes, the former value of `_tmpVehicles` is written back, thus invalidating the contribution of all the threads who had write access in the mean time. Interestingly, when `sleep_for` is commented out, the output of the program is the same as with `std::launch::deferred` - at least that will be the case for most of the time when we run the program. But once in a while, there might be a scheduling constellation which causes the bug to expose itself. Apart from understanding the data race, you

## Using mutex to protect data[¶](https://svefnyqeq7.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Using-mutex-to-protect-data)

In its simplest form, using a mutex consists of four straight-forward steps:

1. Include the `<mutex>` header
2. Create an `std::mutex`
3. Lock the mutex using `lock()` before read/write is called
4. Unlock the mutex after the read/write operation is finished using `unlock()`

In order to protect the access to `_vehicles` from being manipulated by several threads at once, a mutex has been added to the class as a private data member. In the `pushBack` function, the mutex is locked before a new element is added to the vector and unlocked after the operation is complete.

Note that the mutex is also locked in the function printSize just before printing the size of the vector. The reason for this lock is two-fold: First, we want to prevent a data race that would occur when a read-access to the vector and a simultaneous write access (even when under the lock) would occur. And second, we want to exclusively reserve the standard output to the console for printing the vector size without other threads printing to it at the same time.

When this code is executed, 1000 elements will be in the vector. By using a mutex to our shared resource, a data race has been effectively avoided.

![[Pasted image 20231025191401.png]]

## Using timed_mutex[](https://svefnyqeq7.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Using-timed_mutex)

In the following, a short overview of the different available mutex types is given:

- `mutex`: provides the core functions lock() and unlock() and the non-blocking try_lock() method that returns if the mutex is not available.
- `recursive_mutex`: allows multiple acquisitions of the mutex from the same thread.
- `timed_mutex`: similar to mutex, but it comes with two more methods try_lock_for() and try_lock_until() that try to acquire the mutex for a period of time or until a moment in time is reached.
- `recursive_timed_mutex`: is a combination of timed_mutex and recursive_mutex.

## Deadlock 1[](https://svefnyqeq7.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Deadlock-1)

Using mutexes can significantly reduce the risk of data races as seen in the example above. But imagine what would happen if an exception was thrown while executing code in the critical section, i.e. between lock and unlock. In such a case, the mutex would remain locked indefinitely and no other thread could unlock it - the program would most likely freeze.

Let us take a look at the following code example, which performs a division of numbers:
![[Pasted image 20231025194845.png]]
In this example, a number of tasks is started up in `main()` with the method `divideByNumber` as the thread function. Each task is given a different denominator and within divideByNumber a check is performed to avoid a division by zero. If denom should be zero, an exception is thrown. In the catch-block, the exception is caught, printed to the console and then the function returns immediately. The output of the program changes with each execution and might look like this:
![[Pasted image 20231025194854.png]]
As can easily be seen, the console output is totally mixed up and some results appear multiple times. There are several issues with this program, so let us look at them in turn:

1. First, the thread function writes its result to a global variable which is passed to it by reference. This will cause a data race as illustrated in the last section. The `sleep_for` function exposes the data race clearly.
2. Second, the result is printed to the console by several threads at the same time, causing the chaotic output.

## Deadlock 2[](https://svefnyqeq7.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Deadlock-2)

A second type of deadlock is a state in which two or more threads are blocked because each thread waits for the resource of the other thread to be released before releasing its resource. The result of the deadlock is a complete standstill. The thread and therefore usually the whole program is blocked forever. The following code illustrates the problem:
![[Pasted image 20231025195150.png]]
When the program is executed, it produces the following output:
![[Pasted image 20231025195200.png]]
Notice that it does not print the "Finished" statement nor does it return - the program is in a deadlock, which it can never leave.

Let us take a closer look at this problem:

`ThreadA` and `ThreadB` both require access to the console. Unfortunately, they request this resource which is protected by two mutexes in different order. If the two threads work interlocked so that first `ThreadA` locks mutex 1, then `ThreadB` locks mutex 2, the program is in a deadlock: Each thread tries to lock the other mutex and needs to wait for its release, which never comes. The following figure illustrates the problem graphically.

![[Pasted image 20231025195212.png]]

Deadlock example with solution
![](https://youtu.be/5TXfGthAhmU)

# Using Locks to Avoid Deadlocks[](https://eqjfeidiwa.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Using-Locks-to-Avoid-Deadlocks)

## Lock Guard[](https://eqjfeidiwa.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Lock-Guard)

In the previous example, we have directly called the `lock()` and `unlock()` functions of a mutex. The idea of "working under the lock" is to block unwanted access by other threads to the same resource. Only the thread which acquired the lock can unlock the mutex and give all remaining threads the chance to acquire the lock. In practice however, direct calls to `lock()` should be avoided at all cost! Imagine that while working under the lock, a thread would throw an exception and exit the critical section without calling the unlock function on the mutex. In such a situation, the program would most likely freeze as no other thread could acquire the mutex any more. This is exactly what we have seen in the function `divideByNumber` from the previous example.

We can avoid this problem by creating a `std::lock_guard` object, which keeps an associated mutex locked during the entire object life time. The lock is acquired on construction and released automatically on destruction. This makes it impossible to forget unlocking a critical section. Also, `std::lock_guard` guarantees exception safety because any critical section is automatically unlocked when an exception is thrown. In our previous example, we can simply replace `_mutex.lock()` and `_mutex.unlock()` with the following code:
![[Pasted image 20231025200743.png]]

Note that there is no direct call to lock or unlock the mutex anymore. We now have a std::lock_guard object that takes the mutex as an argument and locks it at creation. When the method divideByNumber exits, the mutex is automatically unlocked by the std::lock_guard object as soon as it is destroyed - which happens, when the local variable gets out of scope.

## Unique Lock[](https://eqjfeidiwa.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Unique-Lock)

The problem with the previous example is that we can only lock the mutex once and the only way to control lock and unlock is by invalidating the scope of the std::lock_guard object. But what if we wanted (or needed) a finer control of the locking mechanism?

A more flexible alternative to std::lock_guard is _unique_lock_, that also provides support for more advanced mechanisms, such as deferred locking, time locking, recursive locking, transfer of lock ownership and use of condition variables (which we will discuss later). It behaves similar to lock_guard but provides much more flexibility, especially with regard to the timing behavior of the locking mechanism.

Let us take a look at an adapted version of the code from the previous section above:
![[Pasted image 20231025201055.png]]
In this version of the code, `std::lock_guard` has been replaced with `std::unique_lock`. As before, the lock object `lck` will unlock the mutex in its destructor, i.e. when the function `divideByNumber` returns and `lck` gets out of scope. In addition to this automatic unlocking, `std::unique_lock` offers the additional flexibility to engage and disengage the lock as needed by manually calling the methods `lock()` and `unlock()`. This ability can greatly improve the performance of a concurrent program, especially when many threads are waiting for access to a locked resource. In the example, the lock is released before some non-critical work is performed (simulated by `sleep_for`) and re-engaged before some other work is performed in the critical section and thus under the lock again at the end of the function. This is particularly useful for optimizing performance and responsiveness when a significant amount of time passes between two accesses to a critical resource.

The main advantages of using `std::unique_lock<>` over `std::lock_guard` are briefly summarized in the following. Using `std::unique_lock` allows you to…

1. …construct an instance without an associated mutex using the default constructor
2. …construct an instance with an associated mutex while leaving the mutex unlocked at first using the deferred-locking constructor
3. …construct an instance that tries to lock a mutex, but leaves it unlocked if the lock failed using the try-lock constructor
4. …construct an instance that tries to acquire a lock for either a specified time period or until a specified point in time

Despite the advantages of `std::unique_lock<>` and `std::lock_guard` over

## Avoiding deadlocks with `std::lock()`[](https://eqjfeidiwa.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Avoiding-deadlocks-with-std::lock())

In most cases, your code should only hold one lock on a mutex at a time. Occasionally you can nest your locks, for example by calling a subsystem that protects its internal data with a mutex while holding a lock on another mutex, but it is generally better to avoid locks on multiple mutexes at the same time, if possible. Sometimes, however, it is necessary to hold a lock on more than one mutex because you need to perform an operation on two different data elements, each protected by its own mutex.

In the last section, we have seen that using several mutexes at once can lead to a deadlock, if the order of locking them is not carefully managed. To avoid this problem, the system must be told that both mutexes should be locked at the same time, so that one of the threads takes over both locks and blocking is avoided. That's what the `std::lock()` function is for - you provide a set of `lock_guard` or `unique_lock` objects and the system ensures that they are all locked when the function returns.

In the following example, which is a version of the code we saw in the last section were `std::mutex` has been replaced with `std::lock_guard`.

![[Pasted image 20231025201254.png]]

In the following deadlock-free code, `std::lock` is used to ensure that the mutexes are always locked in the same order, regardless of the order of the arguments. Note that `std::adopt_lock` option allows us to use `std::lock_guard` on an already locked mutex.
![[Pasted image 20231025201427.png]]
As a rule of thumb, programmers should try to avoid using several mutexes at once. Practice shows that this can be achieved in the majority of cases. For the remaining cases though, using `std::lock` is a safe way to avoid a deadlock situation.