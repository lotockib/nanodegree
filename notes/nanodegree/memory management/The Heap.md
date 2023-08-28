## Heap Memory (or Dynamic Memory, Free Store)

Heap memory, also know as dynamic memory , is an important resource available to programs (and programmers) to store data. The following diagram again shows the layout of virtual memory with the heap being right above the BSS and Data segment.

As mentioned earlier, the heap memory grows upwards while the stack grows in the opposite direction. We have seen in the last lesson that the automatic stack memory shrinks and grows with each function call and local variable. As soon as the scope of a variable is left, it is automatically deallocated and the stack pointer is shifted upwards accordingly.

Heap memory is different in many ways: The programmer can request the allocation of memory by issuing a command such as malloc or new (more on that shortly). This block of memory will remain allocated until the programmer explicitly issues a command such as free or delete. The huge advantage of heap memory is the high degree of control a programmer can exert, albeit at the price of greater responsibility since memory on the heap must be actively managed.

Let us take a look at some properties of heap memory:

As opposed to local variables on the stack, memory can now be allocated in an arbitrary scope (e.g. inside a function) without it being deleted when the scope is left. Thus, as long as the address to an allocated block of memory is returned by a function, the caller can freely use it.

Local variables on the stack are allocated at compile-time. Thus, the size of e.g. a string variable might not be appropriate as the length of the string will not be known until the program is executed and the user inputs it. With local variables, a solution would be to allocate a long-enough array of and hope that the actual length does not exceed the buffer size. With dynamically allocated heap memory, variables are allocated at run-time. This means that the size of the above-mentioned string variable can be tailored to the actual length of the user input.

Heap memory is only constrained by the size of the address space and by the available memory. With modern 64 bit operating systems and large RAM memory and hard disks the programmer commands a vast amount of memory. However, if the programmer forgets to deallocate a block of heap memory, it will remain unused until the program is terminated. This is called a "memory leak".

Unlike the stack, the heap is shared among multiple threads, which means that memory management for the heap needs to take concurrency into account as several threads might compete for the same memory resource.

When memory is allocated or deallocated on the stack, the stack pointer is simply shifted upwards or downwards. Due to the sequential structure of stack memory management, stack memory can be managed (by the operating system) easily and securely. With heap memory, allocation and deallocation can occur arbitrarily, depending on the lifetime of the variables. This can result in fragmented memory over time, which is much more difficult and expensive to manage.

### Memory Fragmentation

Let us construct a theoretic example of how memory on the heap can become fragmented: Suppose we are interleaving the allocation of two data types X and Y in the following fashion: First, we allocate a block of memory for a variable of type X, then another block for Y and so on in a repeated manner until some upper bound is reached. At the end of this operation, the heap might look like the following:
![[Pasted image 20230827210238.png]]

At some point, we might then decide to deallocate all variables of type Y, leading to empty spaces in between the remaining variables of type X. In between two blocks of type "X", no memory for an additional "X" could now be squeezed in this example.
![[Pasted image 20230827210241.png]]

A classic symptom of memory fragmentation is that you try to allocate a large block and you can’t, even though you appear to have enough memory free. On systems with virtual memory however, this is less of a problem, because large allocations only need to be contiguous in virtual address space, not in physical address space.

When memory is heavily fragmented however, memory allocations will likely take longer because the memory allocator has to do more work to find a suitable space for the new object.

## Using Malloc and Calloc

User can reserve memory for use with program.

## Freeing up Memory

If memory has been reserved, it should also be released as soon as it is no longer needed. If memory is reserved regularly without releasing it again, the memory capacity may be exhausted at some point. If the RAM memory is completely used up, the data is swapped out to the hard disk, which slows down the computer significantly.

The free function releases the reserved memory area so that it can be used again or made available to other programs. To do this, the pointer pointing to the memory area to be freed is specified as a parameter for the function. In the free_example.cpp, a memory area is reserved and immediately released again.

Some things should be considered with dynamic memory management, whose neglect in some cases might result in unpredictable program behavior or a system crash - in some cases unfortunately without error messages from the compiler or the operating system:

free can only free memory that was reserved with malloc or calloc.

free can only release memory that has not been released before. Releasing the same block of memory twice will result in an error.

In the example on the right, a pointer p is copied into a new variable p2, which is then passed to free AFTER the original pointer has been already released.

free(41143,0x1000a55c0) malloc: *** error for object 0x1003001f0: pointer being freed was not allocated.

In the workspace, you will see this error:

*** Error in './a.out': double free or corruption (fasttop): 0x0000000000755010 ***

The pointer p2 in the example is invalid as soon as free(p) is called. It still holds the address to the memory location which has been freed, but may not access it anymore. Such a pointer is called a "dangling pointer".

Memory allocated with malloc or calloc is not subject to the familiar rules of variables in their respective scopes. This means that they exist independently of block limits until they are released again or the program is terminated. However, the pointers which refer to such heap-allocated memory are created on the stack and thus only exist within a limited scope. As soon as the scope is left, the pointer variable will be lost - but not the heap memory it refers to.

## New and Delete

New/delete calls the constructor automatically. They are also operators, not functions, so they can be overloaded for custom behavior.

Reasons for overloading new and delete

Now that we have seen how to overload the new and delete operators, let us summarize the major scenarios where it makes sense to do this:

The overloaded new operator function allows to add additional parameters. Therefore, a class can have multiple overloaded new operator functions. This gives the programmer more flexibility in customizing the memory allocation for objects.

Overloaded the new and delete operators provides an easy way to integrate a mechanism similar to garbage collection capabilities (such as in Java), as we will shorty see later in this course.

By adding exception handling capabilities into new and delete, the code can be made more robust.

It is very easy to add customized behavior, such as overwriting deallocated memory with zeros in order to increase the security of critical application data.

## Gotchas

- malloc/free with classes does not call contstructor/destructor -> can lead to seg faults
- if free is used, and then memory is accessed after, you get undefined behavior

## Memory Management Problems

Valgrind is one tool

## Rule of 5 (Extension of Rule of 3)

The Rule of Five¶ By adding both the move constructor and the move assignment operator to our MyMovableClass, we have adhered to the Rule of Five. This rule is an extension of the Rule of Three which we have already seen and exists since the introduction of the C++11 standard. The Rule of Five is especially important in resource management, where unnecessary copying needs to be avoided due to limited resources and performance reasons. Also, all the STL container classes such as std::vector implement the Rule of Five and use move semantics for increased efficiency.

The Rule of Five states that if you have to write one of the functions listed below then you should consider implementing all of them with a proper resource management policy in place. If you forget to implement one or more, the compiler will usually generate the missing ones (without a warning) but the default versions might not be suitable for the purpose you have in mind. The five functions are:

The destructor: Responsible for freeing the resource once the object it belongs to goes out of scope.

The assignment operator: The default assignment operation performs a member-wise shallow copy, which does not copy the content behind the resource handle. If a deep copy is needed, it has be implemented by the programmer.

The copy constructor: As with the assignment operator, the default copy constructor performs a shallow copy of the data members. If something else is needed, the programmer has to implement it accordingly.

The move constructor: Because copying objects can be an expensive operation which involves creating, copying and destroying temporary objects, rvalue references are used to bind to an rvalue. Using this mechanism, the move constructor transfers the ownership of a resource from a (temporary) rvalue object to a permanent lvalue object.

The move assignment operator: With this operator, ownership of a resource can be transferred from one object to another. The internal behavior is very similar to the move constructor.

## When are move semantics used?

Now that we have seen how move semantics work, let us take a look at situations where they actually apply.

One of the primary areas of application are cases, where heavy-weight objects need to be passed around in a program. Copying these without move semantics can cause series performance issues. The idea in this scenario is to create the object a single time and then "simply" move it around using rvalue references and move semantics.

A second area of application are cases where ownership needs to be transferred (such as with unique pointers, as we will soon see). The primary difference to shared references is that with move semantics we are not sharing anything but instead we are ensuring through a smart policy that only a single object at a time has access to and thus owns the resource.