#include "MemorySystem.h"

HeapManager *        MemorySystem::vDefaultHeap        = nullptr;
FixedSizeAllocator * MemorySystem::vFSAs[]             = {nullptr, nullptr, nullptr};
bool				 MemorySystem::vAreFSAsConstructed = false;


bool 
MemorySystem::InitializeMemorySystem(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors) noexcept
{
		void * memory;

	vDefaultHeap = HeapManager::CreateHeapManager (i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);

	vAreFSAsConstructed = false;

	for (UInt8 i = 0; i < NUM_FSAS; ++i) {
	
		vFSAs[i] = (FixedSizeAllocator *) vDefaultHeap->Alloc (sizeof (FixedSizeAllocator));

		if (!vFSAs[i])
			return false;

		memory = vDefaultHeap->Alloc (sSizeRequiredForPools[i].uHeapSizeReq);

		if(!memory) {
		
			free (vFSAs[i]);
			return false;
		}
			
		vFSAs[i]->InitializePool (memory, sSizeRequiredForPools[i].uHeapSizeReq, sSizeRequiredForPools[i].uBlockSize, vDefaultHeap);
	}

	vAreFSAsConstructed = true;

	return true;
}

FixedSizeAllocator *
MemorySystem::FindFSA(size_t pSize) noexcept
{
	if (!vAreFSAsConstructed)
		return nullptr;

	for (UInt8 i = 0; i < NUM_FSAS; ++i) {
	
		if (vFSAs[i]->GetBlockSize () >= pSize)
			return vFSAs[i];
	}
	return nullptr;
}

void* MemorySystem::Alloc (size_t pSize) noexcept
{
	void			   *   memory = nullptr;
	FixedSizeAllocator *   fsa;

	fsa = FindFSA (pSize);

	if (fsa)
		memory = fsa->Alloc ();

	if (fsa == nullptr)
		memory = vDefaultHeap->Alloc (pSize);

	return memory;
}

void
MemorySystem::Free (void * pMemory) noexcept
{
	bool isFreed;

	if (vAreFSAsConstructed) {
	
		for (UInt8 i = 0; i < NUM_FSAS; ++i) {
	
			if (vFSAs[i]->Free (pMemory))
				return;
		}
	}

	isFreed = vDefaultHeap->Free (pMemory);
	assert(isFreed);
}

void 
MemorySystem::Collect() noexcept
{
	vDefaultHeap->Collect ();
}

void 
MemorySystem::DestroyMemorySystem() noexcept
{
	for (UInt8 i = 0; i < NUM_FSAS; ++i) {
	
		vFSAs[i]->DestroyPool (vDefaultHeap);
		vDefaultHeap->Free (vFSAs[i]);
	}

	vDefaultHeap->Destroy ();
}

