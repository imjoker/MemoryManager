#pragma once

#include <stdio.h>
#include <Windows.h>
#include <assert.h>

#include "Basic.hpp"

struct tMemoryBlock {

	bool			  uIsInFreeList;
	void *            uBaseAddress = nullptr;
	Int               uBlockSize;
	tMemoryBlock *    uNextBlock   = nullptr;
	tMemoryBlock *    uPrevBlock   = nullptr;
};

class HeapManager {

	public:

static	HeapManager *		CreateHeapManager			(void * pHeapMemory, size_t pSize, BitData pNumDesc) noexcept;

		template <typename Type = void>	
		Type *				Alloc						(size_t pSize) noexcept;

		void				Collect						() noexcept;

		bool				IsAllocated					(void * pMemory) const noexcept;

		bool				Contains					(const void * pMemory) const noexcept;

		void				Destroy						() noexcept;

		template <typename Type = void>
		bool				Free						(Type *& pMemory) noexcept;

		size_t				GetLargestFreeBlock			() const noexcept;
		size_t				GetTotalFreeMemory			() const noexcept;

static	size_t              AlignDown					(size_t pValue, BitData pAlign) noexcept;

static	size_t              AlignUp						(size_t pValue, BitData pAlign) noexcept;

static	inline bool         IsPowerOfTwo				(unsigned __int64 pValue) noexcept;

	private:

		tMemoryBlock * vFreeList;
		tMemoryBlock * vOutstandingList;
		byte *         vEndAddress;
};

template <typename Type>
Type *
HeapManager::Alloc (size_t pSize) noexcept
{
		tMemoryBlock * allocblock;
		tMemoryBlock * walkblock;
		byte *         buff;

	walkblock = vFreeList;

#if (_WIN64)
	pSize = AlignUp (pSize, 8);
#else
	pSize = AlignUp (pSize, 4);
#endif

	// walk till we find a memory block that's good enough for this alloc request
	while (walkblock && walkblock->uBlockSize < static_cast<__int64>(pSize))
		walkblock = walkblock->uNextBlock;

	// check if we actually got a valid memory block after we broke out of while loop
	if (!walkblock || (walkblock->uBlockSize < static_cast<__int64>(pSize))) {
	
		Collect ();

		walkblock = vFreeList;

		// walk till we find a memory block that's good enough for this alloc request
		while (walkblock && walkblock->uBlockSize < static_cast<__int64>(pSize))
			walkblock = walkblock->uNextBlock;

		if (!walkblock || (walkblock->uBlockSize < static_cast<__int64>(pSize)))
			return nullptr;
	}		

	assert (walkblock->uIsInFreeList);

	allocblock = walkblock;

	if (walkblock->uBlockSize > static_cast<__int64>(pSize)) {
	
		// we have more space than required. break down the block into two

		buff =  (reinterpret_cast<byte *>(walkblock)) + (sizeof (tMemoryBlock) + pSize);

		if ((buff >= vEndAddress) || ((buff + sizeof (tMemoryBlock)) >= vEndAddress) || ((allocblock->uBlockSize - (static_cast<__int64>(sizeof (tMemoryBlock) + pSize))) < 0))
			return nullptr;		

		walkblock                =  *(reinterpret_cast<tMemoryBlock **>(&buff));
		walkblock->uBlockSize    =  allocblock->uBlockSize - (static_cast<__int64>(sizeof (tMemoryBlock) + pSize));
		walkblock->uBaseAddress  =  (static_cast<byte *>(allocblock->uBaseAddress)) + (sizeof (tMemoryBlock) + pSize);
		walkblock->uNextBlock    =  allocblock->uNextBlock;
		walkblock->uPrevBlock    =  allocblock->uPrevBlock;
		walkblock->uIsInFreeList =  true;

		if (walkblock->uNextBlock)
			walkblock->uNextBlock->uPrevBlock = walkblock;
		
		if (walkblock->uPrevBlock)
			walkblock->uPrevBlock->uNextBlock = walkblock;

		vFreeList = walkblock;
	} else {
	
		if (walkblock->uPrevBlock)
			walkblock->uPrevBlock->uNextBlock = walkblock->uNextBlock;
		else
			vFreeList = walkblock->uNextBlock;

		if (walkblock->uNextBlock)
			walkblock->uNextBlock->uPrevBlock = walkblock->uPrevBlock;
	}

	// prefix to outstanding list.

	allocblock->uIsInFreeList     =  false;
	allocblock->uBlockSize        =  pSize;
	allocblock->uNextBlock        =  vOutstandingList;
	allocblock->uPrevBlock        =  nullptr;

	if (vOutstandingList)
		vOutstandingList->uPrevBlock  =  allocblock;
	
	vOutstandingList = allocblock;

	return (static_cast<Type *>(allocblock->uBaseAddress));
}

template <typename Type>
bool
HeapManager::Free (Type *& pMemory) noexcept
{
		byte         * bytebuf;
		tMemoryBlock * memblock;
		tMemoryBlock * outstandingnode;

	bytebuf  =  reinterpret_cast<byte *>(pMemory);
	bytebuf -=  sizeof (tMemoryBlock);

	memblock = *(reinterpret_cast<tMemoryBlock **>(&bytebuf));

	outstandingnode = vOutstandingList;

	// check if the memory is in alloced list.
	while (outstandingnode) {

		assert (!outstandingnode->uIsInFreeList);

		if (outstandingnode == memblock)
			goto success;

		outstandingnode = outstandingnode->uNextBlock;
	}

	return false;

success:

	if (outstandingnode->uPrevBlock)
		outstandingnode->uPrevBlock->uNextBlock = outstandingnode->uNextBlock;
	else
		vOutstandingList = outstandingnode->uNextBlock;

	if (outstandingnode->uNextBlock)
		outstandingnode->uNextBlock->uPrevBlock = outstandingnode->uPrevBlock;

	outstandingnode->uNextBlock    = vFreeList;
	outstandingnode->uPrevBlock    = nullptr;
	outstandingnode->uIsInFreeList = true;
	vFreeList->uPrevBlock          = outstandingnode;
	vFreeList                      = outstandingnode;

	return true;
}

#include "HeapManager.hxx"