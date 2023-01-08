# Final Exam Unit Test

1. All the header files are placed in the "Includes" folder.
2. FixedSizeAllocator
	a. Files path: Includes/FixedSizeAllocator.hpp, FixedSizeAllocator/FixedSizeAllocator.cpp
	b. Used BitArray class to track whether blocks are in use or not.
	c. have a 'bool' returning 'free' function to avoid multiple traversals while finding a block and freeing it from FixedSizeAllocator, reducing it to one single traversal instead of two.
3. BitArray:
	a. Includes/BitArray.hpp, Includes/BitArray.hxx
	b. Optimally implemented to suit the platform it is being built for by using 32-bit array for x86 platform and 64-bit array for x64 platform
	c. Used _BitScanForward() and _BitScanForward64() in FindFirstSetBit and FindFirstUnsetBit to optimally search for the set/unset bit
4. HeapManager:
	a. Files path: Includes/HeapManager.hpp, Includes/HeapManager.hxx, HeapManager/HeapManager.cpp
	b. Always aligning the memory requested to multiples of 4 bytes on x86 platform and 8 bytes on x64 platform.
5. MemorySystem:
	a. Files path: Includes/MemorySystem.hpp, Allocator/MemorySystem.cpp 
	b. Implemented the initialization of memory allocators and the alloc/free functions that uses both HeapManager and FixedSizeAllocators optimally based on the memory size requested in the "MemorySystem" class.
6. Works for both 32-bit and 64-bit platforms. Used pre-processor macros to support this.
7. Used const, inline, and constexpr effectively.
