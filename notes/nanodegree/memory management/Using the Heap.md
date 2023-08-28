## Using Malloc and Calloc

User can reserve memory for use with program.

So far we only considered primitive data types, whose storage space requirement was already fixed at compile time and could be scheduled with the building of the program executable. However, it is not always possible to plan the memory requirements exactly in advance, and it is inefficient to reserve the maximum memory space each time just to be on the safe side. C and C++ offer the option to reserve memory areas during the program execution, i.e. at runtime. It is important that the reserved memory areas are released again at the "appropriate point" to avoid memory leaks. It is one of the major challenges in memory management to always locate this "appropriate point" though.

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

## Reasons for overloading `new` and `delete`

Now that we have seen how to overload the new and delete operators, let us summarize the major scenarios where it makes sense to do this:

1. The overloaded new operator function allows to **add additional parameters**. Therefore, a class can have multiple overloaded new operator functions. This gives the programmer more flexibility in customizing the memory allocation for objects.
    
2. Overloaded the new and delete operators provides an easy way to **integrate a mechanism similar to garbage collection** capabilities (such as in Java), as we will shorty see later in this course.
    
3. By adding **exception handling capabilities** into new and delete, the code can be made more robust.
    
4. It is very easy to add customized behavior, such as overwriting deallocated memory with zeros in order to increase the security of critical application data.

## Gotchas

- malloc/free with classes does not call contstructor/destructor -> can lead to seg faults
- if free is used, and then memory is accessed after, you get undefined behavior

## Memory Management Problems

One of the primary advantages of C++ is the flexibility and control of resources such as memory it gives to the programmer. This advantage is further amplified by a significant increase in the performance of C++ programs compared to other languages such as Python or Java.

However, these advantages come at a price as they demand a high level of experience from the programer. As Bjarne Stroustrup put it so elegantly:

_"C makes it easy to shoot yourself in the foot; C++ makes it harder, but when you do it blows your whole leg off"._

In this chapter, we will look at a collection of typical errors in memory management that you need to watch out for.

1. **Memory Leaks** Memory leaks occur when data is allocated on the heap at runtime, but not properly deallocated. A program that forgets to clear a memory block is said to have a memory leak - this may be a serious problem or not, depending on the circumstances and on the nature of the program. For a program that runs, computes something, and quits immediately, memory leaks are usually not a big concern. Memory leaks are mostly problematic for programs that run for a long time and/or use large data structures. In such a case, memory leaks can gradually fill the heap until allocation requests can no longer be properly met and the program stops responding or crashes completely. We will look at an example further down in this section.
    
2. **Buffer Overruns** Buffer overruns occur when memory outside the allocated limits is overwritten and thus corrupted. One of the resulting problems is that this effect may not become immediately visible. When a problem finally does occur, cause and effect are often hard to discern. It is also sometimes possible to inject malicious code into programs in this way, but this shall not be discussed here.
    

In this example, the allocated stack memory is too small to hold the entire string, which results in a segmentation fault:

`char str[5]; strcpy(str,"BufferOverrun"); printf("%s",str);`

3. **Uninitialized Memory** Depending on the C++ compiler, data structures are sometimes initialized (most often to zero) and sometimes not. So when allocating memory on the heap without proper initialization, it might sometimes contain garbage that can cause problems.

Generally, a variable will be automatically initialized in these cases:

- it is a class instance where the default constructor initializes all primitive types
- array initializer syntax is used, such as int a[10] = {}
- it is a global or extern variable
- it is defined `static`

The behavior of the following code is potentially undefined:

`int a; int b=a*42; printf("%d",b);`

4. **Incorrect pairing of allocation and deallocation** Freeing a block of memory more than once will cause a program to crash. This can happen when a block of memory is freed that has never been allocated or has been freed before. Such behavior could also occur when improper pairings of allocation and deallocation are used such as using `malloc()` with `delete` or `new` with `free()`.

In this first example, the wrong `new` and `delete` are paired

`double *pDbl=new double[5]; delete pDbl;`

In this second example, the pairing is correct but a double deletion is performed:

`char *pChr=new char[5]; delete[] pChr; delete[] pChr;`

5. **Invalid memory access** This error occurs then trying to access a block of heap memory that has not yet or has already been deallocated.

In this example, the heap memory has already been deallocated at the time when `strcpy()` tries to access it:

`char *pStr=new char[25]; delete[] pStr; strcpy(pStr, "Invalid Access");`

## Debugging with Valgrind

The Memcheck tool within Valgrind can be used to detect typical errors in programs written in C or C++ that occur in connection with memory management. It is probably the best-known tool in the Valgrind suite, and the name Valgrind is often used as a synonym for Memcheck.

![](https://youtu.be/y0m_6V8fvzA)

# Resource Copy Policies
## Copy Semantics
#todo copy workspace notes here
## Rule of 3

In order to properly manage memory allocation, deallocation and copying behavior, we have seen that there is an intricate relationship between destructor, copy constructor and copy assignment operator. To this end, the **Rule of Three** states that if a class needs 1 of the following to apply special resource allocation rules (such as calling ```delete```, it needs the other 2:

1. The destructor: Responsible for freeing the resource once the object it belongs to goes out of scope.

2. The assignment operator: The default assignment operation performs a member-wise shallow copy, which does not copy the content behind the resource handle. If a deep copy is needed, it has be implemented by the programmer.

3. The copy constructor: As with the assignment operator, the default copy constructor performs a shallow copy of the data members. If something else is needed, the programmer has to implement it accordingly.

## L and R Values
#todo copy workspace notes here

Here are some good resources to learn more about Lvalues and Rvalues:

- [How to crack the confusing world of lvalues and rvalues in C++? It is easy!](https://medium.com/@dhaneshvb/how-to-crack-the-confusing-world-of-lvalues-and-rvalues-in-c-it-is-easy-61c32ced51ce)
- [Lvalues and Rvalues (C++)](https://docs.microsoft.com/en-us/cpp/cpp/lvalues-and-rvalues-visual-cpp?view=msvc-160)

## Move Semantics
#todo copy workspace notes here
## Rule of 5

The Rule of Five is an extension of the Rule of Three.

The Rule of Five By adding both the move constructor and the move assignment operator to our MyMovableClass, we have adhered to the Rule of Five. This rule is an extension of the Rule of Three which we have already seen and exists since the introduction of the C++11 standard. The Rule of Five is especially important in resource management, where unnecessary copying needs to be avoided due to limited resources and performance reasons. Also, all the STL container classes such as std::vector implement the Rule of Five and use move semantics for increased efficiency.

The Rule of Five states that if you have to write one of the functions listed below then you should consider implementing all of them with a proper resource management policy in place. If you forget to implement one or more, the compiler will usually generate the missing ones (without a warning) but the default versions might not be suitable for the purpose you have in mind. The five functions are:

1. The destructor: Responsible for freeing the resource once the object it belongs to goes out of scope.

2. The assignment operator: The default assignment operation performs a member-wise shallow copy, which does not copy the content behind the resource handle. If a deep copy is needed, it has be implemented by the programmer.

3. The copy constructor: As with the assignment operator, the default copy constructor performs a shallow copy of the data members. If something else is needed, the programmer has to implement it accordingly.

4. The move constructor: Because copying objects can be an expensive operation which involves creating, copying and destroying temporary objects, rvalue references are used to bind to an rvalue. Using this mechanism, the move constructor transfers the ownership of a resource from a (temporary) rvalue object to a permanent lvalue object.

5. The move assignment operator: With this operator, ownership of a resource can be transferred from one object to another. The internal behavior is very similar to the move constructor.

## When are move semantics used?

Now that we have seen how move semantics work, let us take a look at situations where they actually apply.

One of the primary areas of application are cases, where heavy-weight objects need to be passed around in a program. Copying these without move semantics can cause series performance issues. The idea in this scenario is to create the object a single time and then "simply" move it around using rvalue references and move semantics.

A second area of application are cases where ownership needs to be transferred (such as with unique pointers, as we will soon see). The primary difference to shared references is that with move semantics we are not sharing anything but instead we are ensuring through a smart policy that only a single object at a time has access to and thus owns the resource.

## Smart Pointers
#todo copy workspace notes here