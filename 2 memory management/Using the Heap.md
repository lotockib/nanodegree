---
trello_plugin_note_id: aCTmTjXoJ4LPox2iUdQqa
trello_board_card_id: 63b438371f268c001585fa84;64ecdc8fa8b9964e99005fa6
---
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
The default behavior of both copy constructor and assignment operator is to perform a _shallow copy_ as with the example above. The following figure illustrates the concept:
![[Pasted image 20230828114456.png]]
Fortunately, in C++, the copying process can be controlled by defining a tailored copy constructor as well as a copy assignment operator. The copying process must be closely linked to the respective resource release mechanism and is often referred to as _copy-ownership policy_. Tailoring the copy constructor according to your memory management policy is an important choice you often need to make when designing a class. In the following, we will closely examine several well-known copy-ownership policies.

## No copying policy[¶](https://4gas2jgb4c.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101692%22%2C%20%22duration%22%3A%20177%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%2232b2f9d9-dbfb-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22uQSo2GVv75w%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_480p_1000kbps.mp4%22%7D%7D%2C%20%7B%22id%22%3A%20%22101691%22%2C%20%22duration%22%3A%20212%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%221f1bca40-dbfb-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22lRRNsOPc6oY%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#No-copying-policy)

The simplest policy of all is to forbid copying and assigning class instances all together. This can be achieved by declaring, but not defining a private copy constructor and assignment operator (see `NoCopyClass1` below) or alternatively by making both public and assigning the `delete` operator (see `NoCopyClass2` below). The second choice is more explicit and makes it clearer to the programmer that copying has been actively forbidden. Let us have a look at a code example on the right that illustrates both cases.

On compiling, we get the following error messages:

`error: calling a private constructor of class 'NoCopyClass1'     NoCopyClass1 copy1(original1);     NoCopyClass1 copy1b = original1;   error: call to deleted constructor of 'NoCopyClass2'     NoCopyClass2 copy2(original2);     NoCopyClass2 copy2b = original2;` 

Both cases effectively prevent the original object from being copied or assigned. In the C++11 standard library, there are some classes for multi-threaded synchronization which use the no copying policy.

## Exclusive ownership policy[¶](https://4gas2jgb4c.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101692%22%2C%20%22duration%22%3A%20177%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%2232b2f9d9-dbfb-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22uQSo2GVv75w%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_480p_1000kbps.mp4%22%7D%7D%2C%20%7B%22id%22%3A%20%22101691%22%2C%20%22duration%22%3A%20212%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%221f1bca40-dbfb-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22lRRNsOPc6oY%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Exclusive-ownership-policy)

This policy states that whenever a resource management object is copied, the resource handle is transferred from the source pointer to the destination pointer. In the process, the source pointer is set to `nullptr` to make ownership exclusive. At any time, the resource handle belongs only to a single object, which is responsible for its deletion when it is no longer needed.

## Deep copying policy[](https://4gas2jgb4c.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101692%22%2C%20%22duration%22%3A%20177%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%2232b2f9d9-dbfb-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22uQSo2GVv75w%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_480p_1000kbps.mp4%22%7D%7D%2C%20%7B%22id%22%3A%20%22101691%22%2C%20%22duration%22%3A%20212%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%221f1bca40-dbfb-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22lRRNsOPc6oY%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Deep-copying-policy)

With this policy, copying and assigning class instances to each other is possible without the danger of resource conflicts. The idea is to allocate proprietary memory in the destination object and then to copy the content to which the source object handle is pointing into the newly allocated block of memory. This way, the content is preserved during copy or assignment. However, this approach increases the memory demands and the uniqueness of the data is lost: After the deep copy has been made, two versions of the same resource exist in memory.

Let us look at an example in the code on the right.

The deep-copy version of MyClass looks similar to the exclusive ownership policy: Both the assignment operator and the copy constructor have been overloaded with the source object passed by reference. But instead of copying the source handle (and then deleting it), a proprietary block of memory is allocated on the heap and the content of the source is copied into it.

The output of the program looks like the following:

`resource allocated at address 0x100300060 resource allocated at address 0x100300070 with _myInt = 42 resource allocated at address 0x100300080 with _myInt = 42 resource freed at address 0x100300080 resource freed at address 0x100300070 resource freed at address 0x100300060`

As can be seen, all copies have the same value of 42 while the address of the handle differs between `source`, `dest1` and `dest2`.

To conclude, the following figure illustrates the idea of a deep copy:
![[Pasted image 20230828114616.png]]

## Shared ownership policy[¶](https://4gas2jgb4c.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101692%22%2C%20%22duration%22%3A%20177%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%2232b2f9d9-dbfb-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22uQSo2GVv75w%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b52_nd213-c03-l04-01.2-copy-semantics-sc/nd213-c03-l04-01.2-copy-semantics-sc_480p_1000kbps.mp4%22%7D%7D%2C%20%7B%22id%22%3A%20%22101691%22%2C%20%22duration%22%3A%20212%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%221f1bca40-dbfb-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22lRRNsOPc6oY%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b31_nd213-c03-l04-02.2-lvalues-and-rvalues-sc/nd213-c03-l04-02.2-lvalues-and-rvalues-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Shared-ownership-policy)

The last ownership policy we will be discussing in this course implements a shared ownership behavior. The idea is to perform a copy or assignment similar to the default behavior, i.e. copying the handle instead of the content (as with a shallow copy) while at the same time keeping track of the number of instances that also point to the same resource. Each time an instance goes out of scope, the counter is decremented. Once the last object is about to be deleted, it can safely deallocate the memory resource. We will see later in this course that this is the central idea of `unique_ptr`, which is a representative of the group of smart pointers.
## Rule of 3

In order to properly manage memory allocation, deallocation and copying behavior, we have seen that there is an intricate relationship between destructor, copy constructor and copy assignment operator. To this end, the **Rule of Three** states that if a class needs 1 of the following to apply special resource allocation rules (such as calling ```delete```, it needs the other 2:

1. The destructor: Responsible for freeing the resource once the object it belongs to goes out of scope.

2. The assignment operator: The default assignment operation performs a member-wise shallow copy, which does not copy the content behind the resource handle. If a deep copy is needed, it has be implemented by the programmer.

3. The copy constructor: As with the assignment operator, the default copy constructor performs a shallow copy of the data members. If something else is needed, the programmer has to implement it accordingly.

## L and R Values
## What are lvalues and rvalues?[](https://kf3nvztsqs.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#What-are-lvalues-and-rvalues?)

A good grasp of lvalues and rvalues in C++ is essential for understanding the more advanced concepts of rvalue references and motion semantics.

Let us start by stating that every expression in C++ has a type and belongs to a value category. When objects are created, copied or moved during the evaluation of an expression, the compiler uses these value expressions to decide which method to call or which operator to use.

Prior to C++11, there were only two value categories, now there are as many as five of them:

![[Pasted image 20230828114825.png]]

To keep it short, we do not want to go into all categories, but limit ourselves to lvalues and prvalues:

- **Lvalues** have an address that can be accessed. They are expressions whose evaluation by the compiler determines the identity of objects or functions.
    
- **Prvalues** do not have an address that is accessible directly. They are temporary expressions used to initialize objects or compute the value of the operand of an operator.
    

For the sake of simplicity and for compliance with many tutorials, videos and books about the topic, let us refer to _prvalues_ as _rvalues_ from here on.

The two characters `l` and `r` are originally derived from the perspective of the assignment operator `=`, which always expects a rvalue on the right, and which it assigns to a lvalue on the left. In this case, the `l` stands for left and `r` for right:

`int i = 42; // lvalue = rvalue;`

With many other operators, however, this right-left view is not entirely correct. In more general terms, an lvalue is an entity that points to a specific memory location. An rvalue is usually a short-lived object, which is only needed in a narrow local scope. To simplify things a little, one could think of lvalues as _named containers_ for rvalues.

In the example above, the value `42` is an rvalue. It does not have a specific memory address which we know about. The rvalue is assigned to a variable `i` with a specific memory location known to us, which is what makes it an lvalue in this example.

Using the address operator `&` we can generate an lvalue from an rvalue and assign it to another lvalue:

`int *j = &i;`

In this small example, the expression `&i` generates the address of `i` as an rvalue and assigns it to `j`, which is an lvalue now holding the memory location of `i`.

The code on the right illustrates several examples of lvalues and rvalues:

Here are some good resources to learn more about Lvalues and Rvalues:

- [How to crack the confusing world of lvalues and rvalues in C++? It is easy!](https://medium.com/@dhaneshvb/how-to-crack-the-confusing-world-of-lvalues-and-rvalues-in-c-it-is-easy-61c32ced51ce)
- [Lvalues and Rvalues (C++)](https://docs.microsoft.com/en-us/cpp/cpp/lvalues-and-rvalues-visual-cpp?view=msvc-160)

## Lvalue references

An lvalue reference can be considered as an alternative name for an object. It is a reference that binds to an lvalue and is declared using an optional list of specifiers (which we will not further discuss here) followed by the reference declarator `&`. The short code sample on the right declares an integer `i` and a reference `j` which can be used as an alias for the existing object.

The output of the program is  
`i = 3, j = 3`

We can see that the lvalue reference `j` can be used just as `i` can. A change to either `i` or `j` will affect the same memory location on the stack.

## Rvalue references

You already know that an rvalue is a temporary expression which is - among other use-cases, a means of initializing objects. In the call `int i = 42`, 42 is the rvalue.

Let us consider an example similar to the last one, shown on the right.

As before, the function `myFunction` takes an lvalue reference as its argument. In `main`, the call `myFunction(j)` works just fine while `myFunction(42)` as well as `myFunction(j+k)` produces the following compiler error on Mac:

`candidate function not viable: expects an l-value for 1st argument`

and the following error in the workspace with g++:

`error: cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’`

While the number `42` is obviously an rvalue, with `j+k` things might not be so obvious, as `j` and `k` are variables and thus lvalues. To compute the result of the addition, the compiler has to create a temporary object to place it in - and this object is an rvalue.

## Move Semantics

## Rvalue references and std::move[¶](https://44px7xlkmw.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101690%22%2C%20%22duration%22%3A%20181%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%2208d240e2-dbfb-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22OOj78tZfdc4%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_480p_1000kbps.mp4%22%7D%7D%2C%20%7B%22id%22%3A%20%22101689%22%2C%20%22duration%22%3A%20215%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22f6974f15-dbfa-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22H1pJmTY9ANQ%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Rvalue-references-and-std::move)

In order to fully understand the concept of smart pointers in the next lesson, we first need to take a look at a powerful concept introduced with C++11 called _move semantics_.

The last section on lvalues, rvalues and especially rvalue references is an important prerequisite for understanding the concept of moving data structures.

Let us consider the function on the right which takes an rvalue reference as its parameter.

The important message of the function argument of `myFunction` to the programmer is : The object that binds to the rvalue reference `&&val` is yours, it is not needed anymore within the scope of the caller (which is `main`). As discussed in the previous section on rvalue references, this is interesting from two perspectives:

1. Passing values like this **improves performance** as no temporary copy needs to be made anymore and
2. **ownership changes**, since the object the reference binds to has been abandoned by the caller and now binds to a handle which is available only to the receiver. This could not have been achieved with lvalue references as any change to the object that binds to the lvalue reference would also be visible on the caller side.

There is one more important aspect we need to consider: _rvalue references are themselves lvalues._ While this might seem confusing at first glance, it really is the mechanism that enables move semantics: A reference is always defined in a certain context (such as in the above example the variable `val`) . Even though the object it refers to (the number `42`) may be disposable in the context it has been created (the `main` function), it is not disposable in the context of the reference . So within the scope of `myFunction`, `val` is an lvalue as it gives access to the memory location where the number 42 is stored.

Note however that in the above code example we cannot pass an lvalue to `myFunction`, because an rvalue reference cannot bind to an lvalue. The code

`int i = 23; myFunction(i)`

would result in a compiler error. There is a solution to this problem though: The function `std::move` converts an lvalue into an rvalue (actually, to be exact, into an _xvalue_, which we will not discuss here for the sake of clarity), which makes it possible to use the lvalue as an argument for the function:

`int i = 23;  myFunction(std::move(i));`

In doing this, we state that in the scope of `main` we will not use i anymore, which now exists only in the scope of `myFunction`. Using `std::move` in this way is one of the components of move semantics, which we will look into shortly. But first let us consider an example of the **Rule of Three**.

Good video on how to use std::move https://youtu.be/OWNeCTd7yQE
## The move constructor[](https://44px7xlkmw.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101690%22%2C%20%22duration%22%3A%20181%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%2208d240e2-dbfb-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22OOj78tZfdc4%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_480p_1000kbps.mp4%22%7D%7D%2C%20%7B%22id%22%3A%20%22101689%22%2C%20%22duration%22%3A%20215%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22f6974f15-dbfa-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22H1pJmTY9ANQ%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-move-constructor)

The basic idea to optimize the code from the last example is to "steal" the rvalue generated by the compiler during the return-by-value operation and move the expensive data in the source object to the target object - not by copying it but by redirecting the data handles. Moving data in such a way is always cheaper than making copies, which is why programmers are highly encouraged to make use of this powerful tool.

The following diagram illustrates the basic principle of moving a resource from a source object to a destination object:

![[Pasted image 20230828115138.png]]

In order to achieve this, we will be using a construct called _move constructor_, which is similar to the copy constructor with the key difference being the re-use of existing data without unnecessarily copying it. In addition to the move constructor, there is also a move assignment operator, which we need to look at.

## Rule of 5

The Rule of Five is an extension of the Rule of Three.

By adding both the move constructor and the move assignment operator to our `MyMovableClass`, we have adhered to the **Rule of Five**. This rule is an extension of the Rule of Three which we have already seen and exists since the introduction of the C++11 standard. The Rule of Five is especially important in resource management, where unnecessary copying needs to be avoided due to limited resources and performance reasons. Also, all the STL container classes such as `std::vector` implement the Rule of Five and use move semantics for increased efficiency.

The Rule of Five states that if you have to write one of the functions listed below then you should consider implementing all of them with a proper resource management policy in place. If you forget to implement one or more, the compiler will usually generate the missing ones (without a warning) but the default versions might not be suitable for the purpose you have in mind. The five functions are:

1. The **destructor**: Responsible for freeing the resource once the object it belongs to goes out of scope.
    
2. The **assignment operator**: The default assignment operation performs a member-wise shallow copy, which does not copy the content behind the resource handle. If a deep copy is needed, it has be implemented by the programmer.
    
3. The **copy constructor**: As with the assignment operator, the default copy constructor performs a shallow copy of the data members. If something else is needed, the programmer has to implement it accordingly.
    
4. The **move constructor**: Because copying objects can be an expensive operation which involves creating, copying and destroying temporary objects, rvalue references are used to bind to an rvalue. Using this mechanism, the move constructor transfers the ownership of a resource from a (temporary) rvalue object to a permanent lvalue object.
    
5. The **move assignment operator**: With this operator, ownership of a resource can be transferred from one object to another. The internal behavior is very similar to the move constructor.

## When are move semantics used?

Now that we have seen how move semantics work, let us take a look at situations where they actually apply.

One of the primary areas of application are cases, where heavy-weight objects need to be passed around in a program. Copying these without move semantics can cause series performance issues. The idea in this scenario is to create the object a single time and then "simply" move it around using rvalue references and move semantics.

A second area of application are cases where ownership needs to be transferred (such as with unique pointers, as we will soon see). The primary difference to shared references is that with move semantics we are not sharing anything but instead we are ensuring through a smart policy that only a single object at a time has access to and thus owns the resource.

## Moving lvalues[¶](https://44px7xlkmw.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101690%22%2C%20%22duration%22%3A%20181%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%2208d240e2-dbfb-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22OOj78tZfdc4%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855b0c_nd213-c03-l04-03.2-move-semantics-sc/nd213-c03-l04-03.2-move-semantics-sc_480p_1000kbps.mp4%22%7D%7D%2C%20%7B%22id%22%3A%20%22101689%22%2C%20%22duration%22%3A%20215%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22f6974f15-dbfa-11e9-8d55-0242ac11001f%22%2C%20%22youtube_id%22%3A%20%22H1pJmTY9ANQ%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d855aed_nd213-c03-l04-03.3-move-semantics-sc/nd213-c03-l04-03.3-move-semantics-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Moving-lvalues)

There is one final aspect we need to look at: In some cases, it can make sense to treat lvalues like rvalues. At some point in your code, you might want to transfer ownership of a resource to another part of your program as it is not needed anymore in the current scope. But instead of copying it, you want to just move it as we have seen before. The "problem" with our implementation of MyMovableClass is that the call `useObject(obj1)` will trigger the copy constructor as we have seen in one of the last examples. But in order to move it, we would have to pretend to the compiler that `obj1` was an rvalue instead of an lvalue so that we can make an efficient move operation instead of an expensive copy.

There is a solution to this problem in C++, which is `std::move`. This function accepts an lvalue argument and returns it as an rvalue without triggering copy construction. So by passing an object to `std::move` we can force the compiler to use move semantics, either in the form of move constructor or the move assignment operator:

`int main() {     MyMovableClass obj1(100); // constructor      useObject(std::move(obj1));      return 0; }`

Nothing much has changed, apart from `obj1` being passed to the `std::move` function. The output would look like the following:

`CREATING instance of MyMovableClass at 0x7ffeefbff718 allocated with size = 400 bytes  MOVING (c'tor) instance 0x7ffeefbff718 to instance 0x7ffeefbff708  using object 0x7ffeefbff708  DELETING instance of MyMovableClass at 0x7ffeefbff708 DELETING instance of MyMovableClass at 0x7ffeefbff718`

By using std::move, we were able to pass the ownership of the resources within `obj1` to the function `useObject`. The local copy `obj1` in the argument list was created with the move constructor and thus accepted the ownership transfer from `obj1` to `obj` . Note that after the call to `useObject`, the instance `obj1` has been invalidated by setting its internal handle to null and thus may not be used anymore

## Error-prone memory management with `new` and `delete`[¶](https://fhgbrqhtop.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Error-prone-memory-management-with-new-and-delete)

In the previous chapters, we have seen that memory management on the heap using `malloc`/`free` or `new`/`delete` is extremely powerful, as they allow for a fine-grained control over the precious memory resource. However, the correct use of these concepts requires some degree of skill and experience (and concentration) from the programmer. If they are not handled correctly, bugs will quickly be introduced into the code. A major source of error is that the details around memory management with `new`/`delete` are completely left to the programer. In the remainder of this lesson, the pair `malloc`/`free` will be omitted for reasons of brevity. However, many of the aspects that hold for `new`/`delete` will also apply to `malloc`/`free`.

Let us take a look at some of the worst problems with `new` and `delete`:

1. **Proper pairing of new and delete** : Every dynamically allocated object that is created with new must be followed by a manual deallocation at a "proper" place in the program. If the programer forgets to call delete (which can happen very quickly) or if it is done at an "inappropriate" position, memory leaks will occur which might clog up a large portion of memory.
    
2. **Correct operator pairing** : C++ offers a variety of `new`/`delete` operators, especially when dealing with arrays on the heap. A dynamically allocated array initialized with `new[]` may only be deleted with the operator `delete[]`. If the wrong operator is used, program behavior will be undefined - which is to be avoided at all cost in C++.
    
3. **Memory ownership** : If a third-party function returns a pointer to a data structure, the only way of knowing who will be responsible for resource deallocation is by looking into either the code or the documentation. If both are not available (as is often the case), there is no way to infer the ownership from the return type. As an example, in the final project of this course, we will use the graphical library wxWidgets to create the user interface of a chatbot application. In wxWidgets, the programmer can create child windows and control elements on the heap using `new`, but the framework will take care of deletion altogether. If for some reason the programmer does not know this, he or she might call delete and thus interfere with the inner workings of the wxWidgets library.
# Smart Pointers

### The benefits of smart pointers[](https://fhgbrqhtop.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-benefits-of-smart-pointers)

To put it briefly: Smart pointers were introduced in C++ to solve the above mentioned problems by providing a degree of automatic memory management: When a smart pointer is no longer needed (which is the case as soon as it goes out of scope), the memory to which it points is automatically deallocated. When contrasted with smart pointers, the conventional pointers we have seen so far are often termed "raw pointers".

In essence, smart pointers are classes that are wrapped around raw pointers. By overloading the `->` and `*` operators, smart pointer objects make sure that the memory to which their internal raw pointer refers to is properly deallocated. This makes it possible to use smart pointers with the same syntax as raw pointers. As soon as a smart pointer goes out of scope, its destructor is called and the block of memory to which the internal raw pointer refers is properly deallocated. This technique of wrapping a management class around a resource has been conceived by Bjarne Stroustroup and is called **Resource Acquisition Is Initialization (RAII)**. Before we continue with smart pointers and their usage let us take a close look at this powerful concept.

### Resource Acquisition Is Initialization[](https://fhgbrqhtop.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Resource-Acquisition-Is-Initialization)

The RAII is a widespread programming paradigm, that can be used to protect a resource such as a file stream, a network connection or a block of memory which need proper management.

#### Acquiring and releasing resources[](https://fhgbrqhtop.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Acquiring-and-releasing-resources)

In most programs of reasonable size, there will be many situations where a certain action at some point will necessitate a proper reaction at another point, such as:

1. Allocating memory with `new` or `malloc`, which must be matched with a call to `delete` or `free`.
    
2. Opening a file or network connection, which must be closed again after the content has been read or written.
    
3. Protecting synchronization primitives such as atomic operations, memory barriers, monitors or critical sections, which must be released to allow other threads to obtain them.
    

The following table gives a brief overview of some resources and their respective allocation and deallocation calls in C++:

![[Pasted image 20230828132204.png]]

#### The problem of reliable resource release[¶](https://fhgbrqhtop.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-problem-of-reliable-resource-release)

A general usage pattern common to the above examples looks like the following:

![[Pasted image 20230828132227.png]]

However, there are several problems with this seemingly simple pattern:

1. The program might throw an exception during resource use and thus the point of release might never be reached.
    
2. There might be several points where the resource could potentially be released, making it hard for a programmer to keep track of all eventualities.
    
3. We might simply forget to release the resource again.

#### RAII to the rescue[](https://fhgbrqhtop.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#RAII-to-the-rescue)

The major idea of RAII revolves around object ownership and information hiding: Allocation and deallocation are hidden within the management class, so a programmer using the class does not have to worry about memory management responsibilities. If he has not directly allocated a resource, he will not need to directly deallocate it - whoever owns a resource deals with it. In the case of RAII this is the management class around the protected resource. The overall goal is to have allocation and deallocation (e.g. with `new` and `delete`) disappear from the surface level of the code you write.

RAII can be used to leverage - among others - the following advantages:

- Use class destructors to perform resource clean-up tasks such as proper memory deallocation when the RAII object gets out of scope
- Manage ownership and lifetime of dynamically allocated objects
- Implement encapsulation and information hiding due to resource acquisition and release being performed within the same object.

In the following, let us look at RAII from the perspective of memory management. There are three major parts to an RAII class:

1. A resource is allocated in the constructor of the RAII class
2. The resource is deallocated in the destructor
3. All instances of the RAII class are allocated on the stack to reliably control the lifetime via the object scope

#### RAII and smart pointers[¶](https://w3flmxh4bz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101707%22%2C%20%22duration%22%3A%20201%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22925219a8-dc8f-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22Sjq5L4o8lAo%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#RAII-and-smart-pointers)

In the last section, we have discussed the powerful RAII idiom, which reduces the risk of improperly managed resources. Applied to the concept of memory management, RAII enables us to encapsulate `new` and `delete` calls within a class and thus present the programmer with a clean interface to the resource he intends to use. Since C++11, there exists a language feature called _smart pointers_, which builds on the concept of RAII and - without exaggeration - revolutionizes the way we use resources on the heap. Let’s take a look.

### Smart pointer overview[](https://w3flmxh4bz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101707%22%2C%20%22duration%22%3A%20201%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22925219a8-dc8f-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22Sjq5L4o8lAo%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#Smart-pointer-overview)

Since C++11, the standard library includes _smart pointers_, which help to ensure that programs are free of memory leaks while also remaining exception-safe. With smart pointers, resource acquisition occurs at the same time that the object is initialized (when instantiated with `make_shared` or `make_unique`), so that all resources for the object are created and initialized in a single line of code.

In modern C++, raw pointers managed with `new` and `delete` should only be used in small blocks of code with limited scope, where performance is critical (such as with `placement new`) and ownership rights of the memory resource are clear. We will look at some guidelines on where to use which pointer later.

C++11 has introduced three types of smart pointers, which are defined in the header of the standard library:

1. The **unique pointer** `std::unique_ptr` is a smart pointer which exclusively owns a dynamically allocated resource on the heap. There must not be a second unique pointer to the same resource.
    
2. The **shared pointer** `std::shared_ptr` points to a heap resource but does not explicitly own it. There may even be several shared pointers to the same resource, each of which will increase an internal reference count. As soon as this count reaches zero, the resource will automatically be deallocated.
    
3. The **weak pointer** `std::weak_ptr` behaves similar to the shared pointer but does not increase the reference counter.
    

Prior to C++11, there was a concept called `std::auto_ptr`, which tried to realize a similar idea. However, this concept can now be safely considered as deprecated and should not be used anymore.

Let us now look at each of the three smart pointer types in detail

### Types of Smart Pointers
#### The Unique pointer[](https://w3flmxh4bz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101707%22%2C%20%22duration%22%3A%20201%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22925219a8-dc8f-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22Sjq5L4o8lAo%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-Unique-pointer)

A unique pointer is the exclusive owner of the memory resource it represents. There must not be a second unique pointer to the same memory resource, otherwise there will be a compiler error. As soon as the unique pointer goes out of scope, the memory resource is deallocated again. Unique pointers are useful when working with a temporary heap resource that is no longer needed once it goes out of scope.

The following diagram illustrates the basic idea of a unique pointer:

![[Pasted image 20230828132517.png]]

In the example, a resource in memory is referenced by a unique pointer instance `sourcePtr`. Then, the resource is reassigned to another unique pointer instance `destPtr` using `std::move`. The resource is now owned by `destPtr` while `sourcePtr` can still be used but does not manage a resource anymore.

A unique pointer is constructed using the following syntax:

`std::unique_ptr<Type> p(new Type);`

Summing up, the unique pointer allows a single owner of the underlying internal raw pointer. Unique pointers should be the default choice unless you know for certain that sharing is required at a later stage. We have already seen how to transfer ownership of a resource using the Rule of Five and move semantics. Internally, the unique pointer uses this very concept along with RAII to encapsulate a resource (the raw pointer) and transfer it between pointer objects when either the move assignment operator or the move constructor are called. Also, a key feature of a unique pointer, which makes it so well-suited as a return type for many functions, is the possibility to convert it to a shared pointer. We will have a deeper look into this in the section on ownership transfer.

#### The Shared Pointer[](https://w3flmxh4bz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101707%22%2C%20%22duration%22%3A%20201%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22925219a8-dc8f-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22Sjq5L4o8lAo%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-Shared-Pointer)

Just as the unique pointer, a shared pointer owns the resource it points to. The main difference between the two smart pointers is that shared pointers keep a reference counter on how many of them point to the same memory resource. Each time a shared pointer goes out of scope, the counter is decreased. When it reaches zero (i.e. when the last shared pointer to the resource is about to vanish). the memory is properly deallocated. This smart pointer type is useful for cases where you require access to a memory location on the heap in multiple parts of your program and you want to make sure that whoever owns a shared pointer to the memory can rely on the fact that it will be accessible throughout the lifetime of that pointer.

The following diagram illustrates the basic idea of a shared pointer:

![[Pasted image 20230828132647.png]]

#### The Weak Pointer[](https://w3flmxh4bz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101707%22%2C%20%22duration%22%3A%20201%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22925219a8-dc8f-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22Sjq5L4o8lAo%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#The-Weak-Pointer)

Similar to shared pointers, there can be multiple weak pointers to the same resource. The main difference though is that weak pointers do not increase the reference count. Weak pointers hold a non-owning reference to an object that is managed by another shared pointer.

The following rule applies to weak pointers: You can only create weak pointers out of shared pointers or out of another weak pointer. The code on the right shows a few examples of how to use and how not to use weak pointers.

The output looks as follows:

`shared pointer count = 1 shared pointer count = 1`

First, a shared pointer to an integer is created with a reference count of 1 after creation. Then, two weak pointers to the integer resource are created, the first directly from the shared pointer and the second indirectly from the first weak pointer. As can be seen from the output, neither of both weak pointers increased the reference count. At the end of main, the attempt to directly create a weak pointer to an integer resource would lead to a compile error.

### When to use raw pointers and smart pointers?[](https://w3flmxh4bz.prod.udacity-student-workspaces.com/lab?ulab=%7B%22disk%22%3A%20null%2C%20%22port%22%3A%203000%2C%20%22ports%22%3A%20%5B%5D%2C%20%22videos%22%3A%20%5B%7B%22id%22%3A%20%22101707%22%2C%20%22duration%22%3A%20201%2C%20%22subtitles%22%3A%20%5B%5D%2C%20%22topher_id%22%3A%20%22925219a8-dc8f-11e9-b7ab-0242ac11000d%22%2C%20%22youtube_id%22%3A%20%22Sjq5L4o8lAo%22%2C%20%22transcodings%22%3A%20%7B%22uri_hls%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/hls/playlist.m3u8%22%2C%20%22uri_480p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p.mp4%22%2C%20%22uri_720p_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_720p.mp4%22%2C%20%22uri_480p_1000kbps_mp4%22%3A%20%22https%3A//video.udacity-data.com/topher/2019/September/5d865440_nd213-c03-l05-02.2-smart-pointers-sc/nd213-c03-l05-02.2-smart-pointers-sc_480p_1000kbps.mp4%22%7D%7D%5D%2C%20%22pageEnd%22%3A%20%22%22%2C%20%22pageStart%22%3A%20%22%22%2C%20%22allowSubmit%22%3A%20false%2C%20%22defaultPath%22%3A%20%22/%22%2C%20%22actionButtonText%22%3A%20%22%22%7D#When-to-use-raw-pointers-and-smart-pointers?)

As a general rule of thumb with modern C++, smart pointers should be used often. They will make your code safer as you no longer need to think (much) about the proper allocation and deallocation of memory. As a consequence, there will be much fewer memory leaks caused by dangling pointers or crashes from accessing invalidated memory blocks.

When using raw pointers on the other hand, your code might be susceptible to the following bugs:

1. Memory leaks
2. Freeing memory that shouldn’t be freed
3. Freeing memory incorrectly
4. Using memory that has not yet been allocated
5. Thinking that memory is still allocated after being freed

With all the advantages of smart pointers in modern C++, one could easily assume that it would be best to completely ban the use of new and delete from your code. However, while this is in many cases possible, it is not always advisable as well. Let us take a look at the [C++ core guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#main), which has several [**rules for explicit memory allocation and deallocation**](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r-resource-management). In the scope of this course, we will briefly discuss three of them:

1. **R. 10: Avoid malloc and free** While the calls `(MyClass*)malloc( sizeof(MyClass) )` and `new MyClass` both allocate a block of memory on the heap in a perfectly valid manner, only `new` will also call the constructor of the class and `free` the destructor. To reduce the risk of undefined behavior, `malloc` and `free` should thus be avoided.
    
2. **R. 11: Avoid calling new and delete explicitly** Programmers have to make sure that every call of `new` is paired with the appropriate `delete` at the correct position so that no memory leak or invalid memory access occur. The emphasis here lies in the word "explicitly" as opposed to implicitly, such as with smart pointers or containers in the standard template library.
    
3. **R. 12: Immediately give the result of an explicit resource allocation to a manager object** It is recommended to make use of manager objects for controlling resources such as files, memory or network connections to mitigate the risk of memory leaks. This is the core idea of smart pointers as discussed at length in this section.
    

Summarizing, raw pointers created with `new` and `delete` allow for a high degree of flexibility and control over the managed memory as we have seen in earlier lessons of this course. To mitigate their proneness to errors, the following additional recommendations can be given:

- A call to `new` should not be located too far away from the corresponding `delete`. It is bad style to stretch you `new` / `delete` pairs throughout your program with references criss-crossing your entire code.
    
- Calls to `new` and `delete` should always be hidden from third parties so that they must not concern themselves with managing memory manually (which is similar to R. 12).

In addition to the above recommendations, the C++ core guidelines also contain a total of 13 rules for the [**recommended use of smart pointers**](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rsmart-smart-pointers). In the following, we will discuss a selection of these:

1. **R. 20 : Use unique_ptr or shared_ptr to represent ownership**
    
2. **R. 21 : Prefer unique_ptr over std::shared_ptr unless you need to share ownership**
    

Both pointer types express ownership and responsibilities (R. 20). A `unique_ptr` is an exclusive owner of the managed resource; therefore, it cannot be copied, only moved. In contrast, a `shared_ptr` shares the managed resource with others. As described above, this mechanism works by incrementing and decrementing a common reference counter. The resulting administration overhead makes `shared_ptr` more expensive than `unique_ptr`. For this reason `unique_ptr` should always be the first choice (R. 21).

4. **R. 22 : Use make_shared() to make shared_ptr**
    
5. **R. 23 : Use make_unique() to make std::unique_ptr**
    

The increased management overhead compared to raw pointers becomes in particular true if a `shared_ptr` is used. Creating a `shared_ptr` requires (1) the allocation of the resource using new and (2) the allocation and management of the reference counter. Using the factory function `make_shared` is a one-step operation with lower overhead and should thus always be preferred. (R.22). This also holds for `unique_ptr` (R.23), although the performance gain in this case is minimal (if existent at all).

But there is an additional reason for using the `make_...` factory functions: Creating a smart pointer in a single step removes the risk of a memory leak. Imagine a scenario where an exception happens in the constructor of the resource. In such a case, the object would not be handled properly and its destructor would never be called - even if the managing object goes out of scope. Therefore, `make_shared` and `make_unique` should always be preferred. Note that `make_unique` is only available with compilers that support at least the C++14 standard.

3. **R. 24 : Use weak_ptr to break cycles of shared_ptr**

We have seen that weak pointers provide a way to break a deadlock caused by two owning references which are cyclicly referring to each other. With weak pointers, a resource can be safely deallocated as the reference counter is not increased.

The remaining set of guideline rules referring to smart pointers are mostly concerning the question of how to pass a smart pointer to a function. We will discuss this question in the next concept.