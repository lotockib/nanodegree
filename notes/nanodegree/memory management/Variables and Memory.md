## Process Memory Model

When a process is run, virtual memory is allocated to it. That is divided into 6 areas. The OS controls some areas. The progreammer is responsible for handling heap vs stack.

The stack is a contiguous memory block with a fixed maximum size. If a program exceeds this size, it will crash. The stack is used for storing automatically allocated variables such as local variables or function parameters. If there are multiple threads in a program, then each thread has its own stack memory. New memory on the stack is allocated when the path of execution enters a scope and freed again once the scope is left. It is important to know that the stack is managed "automatically" by the compiler, which means we do not have to concern ourselves with allocation and deallocation.

The heap (also called "free store" in C++) is where data with dynamic storage lives. It is shared among multiple threads in a program, which means that memory management for the heap needs to take concurrency into account. This makes memory allocations in the heap more complicated than stack allocations. In general, managing memory on the heap is more (computationally) expensive for the operating system, which makes it slower than stack memory. Contrary to the stack, the heap is not managed automatically by the system, but by the programmer. If memory is allocated on the heap, it is the programmer’s responsibility to free it again when it is no longer needed. If the programmer manages the heap poorly or not at all, there will be trouble.

The BSS (Block Started by Symbol) segment is used in many compilers and linkers for a segment that contains global and static variables that are initialized with zero values. This memory area is suitable, for example, for arrays that are not initialized with predefined values.

The Data segment serves the same purpose as the BSS segment with the major difference being that variables in the Data segment have been initialized with a value other than zero. Memory for variables in the Data segment (and in BSS) is allocated once when a program is run and persists throughout its lifetime.
![[Pasted image 20230827210137.png]]

## Stack

Pros / Cons

- easier because the OS manages allocation and deallocation
- smaller than heap
- faster than heap
- each thread has it's own stack memory

Properties

- The stack is a contiguous block of memory. It will not become fragmented (as opposed to the heap) and it has a fixed maximum size.
- When the maximum size of the stack memory is exceeded, a program will crash.
- Allocating and deallocating memory is fast on the stack. It only involves moving the stack pointer to a new position.

The stack grows and contracts as the OS automatically allocates and deallocates memory for variables.

Stack overflow is what the entire space of the stack is consumed.

For my machine, `ulimit -s` says I have 8MB of stack.

[example code that causes stack overflow](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/2_memory_management/2_variables_and_memory/lesson_03_stack_overflow.cpp "./lesson_03_stack_overflow.cpp")

### Passing Variables by Value

When calling a function as in the previous code example, its parameters (in this case myInt) are used to create local copies of the information provided by the caller. The caller is not sharing the parameter with the function but instead a proprietary copy is created using the assignment operator = (more about that later). When passing parameters in such a way, it is ensured that changes made to the local copy will not affect the original on the caller side. The upside to this is that inner workings of the function and the data owned by the caller are kept neatly separate.

However, there are two major downsides to this:

Passing parameters by value means that a copy is created, which is an expensive operation that might consume large amounts of memory, depending on the data that is being transferred. Later in this course we will encounter "move semantics", which is an effective way to compensate for this downside. Passing by value also means that the created copy can not be used as a back channel for communicating with the caller, for example by directly writing the desired information into the variable.

You can pass a memory address to a pointer, which allows backchannel communication, but it is still passing variable by reference and making a copy.

### Passing Variables by Reference

The second major way of passing parameters to a function is by reference. With this way, the function receives a reference to the parameter, rather than a copy of its value. As with the example of AddThree above, the function can now modify the argument such that the changes also happen on the caller side. In addition to the possibility to directly exchange information between function and caller, passing variables by reference is also faster as no information needs to be copied, as well as more memory-efficient.

A major disadvantage is that the caller does not always know what will happen to the data it passes to a function (especially when the function code can not be modified easily). Thus, in some cases, special steps must be taken to protect ones data from inappropriate modification.

### Example

Note that sometimes the reference being passed can be larger than the value itself. [lesson example](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/2_memory_management/2_variables_and_memory/lesson_04_ways_to_pass_argument.cpp "./lesson_04_ways_to_pass_argument.cpp"). Let us take a look at the size of the various parameter types using the sizeof command:

printf("size of int: %lu\n", sizeof(int)); printf("size of *int: %lu\n", sizeof(int *)); The output here is

size of int: 4 size of *int: 8 Obviously, the size of the pointer variable is larger than the actual data type. As my machine has a 64 bit architecture, an address requires 8 byte.

As an experiment, you could use the -m32 compiler flag to build a 32 bit version of the program. This yields the following output:

size of int: 4 size of *int: 4 In order to benefit from call-by-reference, the size of the data type passed to the function has to surpass the size of the pointer on the respective architecture (i.e. 32 bit or 64 bit).

## Heap

Pros / Cons

- larger than stack (good for large images like images or videos)
- slower than stack
- has to be manager by user
- if user mismanages, like forgets to deallocate, there can be problems