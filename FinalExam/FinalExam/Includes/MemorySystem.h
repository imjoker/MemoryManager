#pragma once

#include "FixedSizeAllocator.hpp"

struct tFSAInitData {

	UInt32 uHeapSizeReq;
	UInt32 uBlockSize;
};

#define NUM_FSAS 3

class MemorySystem {

public:

	// InitializeMemorySystem - initialize your memory system including your HeapManager and some FixedSizeAllocators
static	bool						InitializeMemorySystem		(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors) noexcept;

static	FixedSizeAllocator *		FindFSA						(size_t pSize) noexcept;


static	void *						Alloc						(size_t pSize) noexcept;

static  void						Free						(void * pMemory) noexcept;

	// Collect - coalesce free blocks in attempt to create larger blocks
static	void						Collect						() noexcept;

	// DestroyMemorySystem - destroy your memory systems
static	void						DestroyMemorySystem			() noexcept;

private:

static	HeapManager *           vDefaultHeap;
static	FixedSizeAllocator *	vFSAs[NUM_FSAS];
static  bool					vAreFSAsConstructed;

static constexpr tFSAInitData sSizeRequiredForPools[NUM_FSAS] {{1600, 16}, {6400, 32}, {38400, 96}};

};

extern void* __cdecl __malloc__(size_t _Size);

extern void __cdecl __free__(void * i_ptr);

extern void * operator new(size_t i_size);

extern void operator delete(void * i_ptr);

extern void * operator new[](size_t i_size);

extern void operator delete [](void * i_ptr);
