#include "FixedSizeAllocator.hpp"

void 
FixedSizeAllocator::InitializePool (void * pMemory, size_t pMemorySize, size_t pBlockSize, HeapManager * pHM) noexcept
{
		UInt32 numblocks;

	vMemoryPool = (Byte *) pMemory;
	vBlockSize  = pBlockSize;
	numblocks   = static_cast<UInt32> (pMemorySize / pBlockSize);

	vAreBlocksFree = (BitArray *) pHM->Alloc (sizeof (BitArray));

	vAreBlocksFree->Init (numblocks, pHM, true);
}

void *
FixedSizeAllocator::Alloc () noexcept
{
		UInt32 index;

	if (vAreBlocksFree->FindFirstSetBit (index)) {

		vAreBlocksFree->Unset (index);

		return ((vMemoryPool) + (index * vBlockSize));
	} else
		return nullptr;
}

bool
FixedSizeAllocator::Free (void * pBlock) noexcept
{
		UInt32 index;
		UInt32 numblocks;
		Byte * temp;

	if (!vAreBlocksFree->FindFirstUnsetBit (index))
		return false;

	numblocks = vAreBlocksFree->GetCapacity ();

	temp = (vMemoryPool + (index * vBlockSize));

	while (numblocks > index) {

		if (temp == pBlock)
			goto found;

		++index;

		temp = temp + vBlockSize;
	}

	return false;

found:
	vAreBlocksFree->Set (index);

	return true;
}

size_t 
FixedSizeAllocator::GetBlockSize() const noexcept
{
	return vBlockSize;
}

void
FixedSizeAllocator::DestroyPool (HeapManager * pHM) noexcept
{
	assert(!vAreBlocksFree->AreAllBitsSet ());

	vAreBlocksFree->Destroy (pHM);

	pHM->Free (vAreBlocksFree);
}