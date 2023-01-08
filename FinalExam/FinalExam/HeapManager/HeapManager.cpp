
#include "HeapManager.hpp"

HeapManager *
HeapManager::CreateHeapManager (void * pHeapMemory, size_t pSize, BitData /*pNumDesc*/) noexcept
{

	HeapManager * hm;
	byte *        bytebuf;

	assert(pHeapMemory);

	bytebuf =  static_cast<byte *>(pHeapMemory);

	hm      =  *(reinterpret_cast<HeapManager **>(&bytebuf));  // access the first few bytes as our heapmanager
	pSize   -= sizeof (HeapManager);			               // reduce the heap size to update available space

	hm->vEndAddress = bytebuf + pSize;

	bytebuf += sizeof (HeapManager);			// move the buffer forward

	// consider the next bytes as our one large free memory block
	hm->vFreeList  = reinterpret_cast<tMemoryBlock *>(bytebuf);
	pSize         -= sizeof (tMemoryBlock);
	bytebuf       += sizeof (tMemoryBlock);

	// set the memory block's data.
	hm->vFreeList->uBlockSize    = pSize;
	hm->vFreeList->uBaseAddress  = bytebuf;
	hm->vFreeList->uIsInFreeList = true;
	hm->vFreeList->uNextBlock    = nullptr;
	hm->vFreeList->uPrevBlock    = nullptr;

	// we don't yet have any outstanding memory blocks
	hm->vOutstandingList = nullptr;

	return hm;
}

void
HeapManager::Collect () noexcept
{
		tMemoryBlock * freenode;
		tMemoryBlock * nextnode;
		byte *         nextnodeloc;

	freenode = vFreeList;

	while (freenode) {

		nextnodeloc = ((static_cast<byte *>(freenode->uBaseAddress)) + freenode->uBlockSize + sizeof (tMemoryBlock));
		nextnode    = *(reinterpret_cast<tMemoryBlock **>(&nextnodeloc));

		if ((nextnodeloc < vEndAddress) && nextnode->uIsInFreeList) {
	
			freenode->uBlockSize += (nextnode->uBlockSize + sizeof (tMemoryBlock));

			if (nextnode->uPrevBlock)
				nextnode->uPrevBlock->uNextBlock = nextnode->uNextBlock;
		
			if (nextnode->uNextBlock)
				nextnode->uNextBlock->uPrevBlock = nextnode->uPrevBlock;
		}

		freenode = freenode->uNextBlock;
	}
}

bool
HeapManager::IsAllocated (void * pMemory) const noexcept
{
	byte         * bytebuf;
	tMemoryBlock * inputmemblock;
	tMemoryBlock * outstandingnode;
	tMemoryBlock * freenode;

	bytebuf        =   static_cast<byte *>(pMemory);
	bytebuf       -=   sizeof (tMemoryBlock);
	inputmemblock  =   reinterpret_cast<tMemoryBlock *>(bytebuf);

	outstandingnode = vOutstandingList;
	freenode        = vFreeList;

	// check if the memory is in alloced list.
	while (outstandingnode) {

		if (outstandingnode == inputmemblock)
			return true;

		outstandingnode = outstandingnode->uNextBlock;
	}

	return false;
}

bool
HeapManager::Contains (const void * pMemory) const noexcept
{
	const byte   *       bytebuf;
	const tMemoryBlock * inputmemblock;
	const tMemoryBlock * outstandingnode;
	const tMemoryBlock * freenode;

	bytebuf        =   static_cast<const byte *>(pMemory);
	bytebuf        -=  sizeof (tMemoryBlock);
	inputmemblock  =   (const tMemoryBlock *) bytebuf;

	outstandingnode = vOutstandingList;
	freenode        = vFreeList;

	// check if the memory is in alloced list.
	while (outstandingnode) {

		if (outstandingnode == inputmemblock)
			return true;

		outstandingnode = outstandingnode->uNextBlock;
	}

	// check if the memory is in free list.
	while (freenode) {

		if (freenode == inputmemblock)
			return true;

		freenode = freenode->uNextBlock;
	}

	// not found
	return false;
}

void
HeapManager::Destroy () noexcept
{
	// nothing to do at the moment
}

size_t 
HeapManager::GetLargestFreeBlock () const noexcept
{
		const tMemoryBlock * allocblock;
		const tMemoryBlock * walkblock;

	allocblock = vFreeList;
	walkblock  = vFreeList->uNextBlock;

	while (walkblock) {
	
		if (walkblock->uBlockSize > allocblock->uBlockSize)
			allocblock = walkblock;
	}

	return allocblock->uBlockSize;
}

size_t
HeapManager::GetTotalFreeMemory () const noexcept
{
		size_t               freememsize;
		const tMemoryBlock * walkblock;

	freememsize = 0;

	walkblock = vFreeList;

	while (walkblock)
		freememsize += walkblock->uBlockSize;

	return freememsize;
}
