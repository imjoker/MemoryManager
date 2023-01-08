#pragma once

#include "BitArray.hpp"

class FixedSizeAllocator {

public:

	void		InitializePool		 (void * pMemory, size_t pMemorySize, size_t pBlockSize, HeapManager * pHM) noexcept;

	void *		Alloc				 () noexcept;

	bool		Free				 (void * pBlock) noexcept;

	size_t		GetBlockSize		 () const noexcept;

	void        DestroyPool          (HeapManager * pHM) noexcept;

private:

	Byte  *    vMemoryPool;
	BitArray * vAreBlocksFree;
	size_t     vBlockSize;
};

