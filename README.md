# 2D Game Engine

- The first part of this engine is where I implemented a memory manager to increase the efficiency of applications, and decrease the number of kernel-level allocation calls, resulting in a cache-friendly framework.
- Majority of the allocation requests are handled by the FixedSizeAllocators of custom sizes, which I implemented using bit-array to track allocations and deallocations.
- The other minor allocations are handled by a HeapManager, which can allocate memory blocks of any size and employs a linked-list technique to track the allocated and free lists.
- Additionally, I utilized Verlet numerical integration to create the Physics system for game objects.
-  I also implemented Smart and Weak pointers to make it easier to manage the destruction of game objects and maintain multiple references to the same object across different engine systems.
