#include <inttypes.h>

#include <stdio.h>

#include "MemorySystem.h"

void* __cdecl __malloc__(size_t _Size)
{
	return MemorySystem::Alloc (_Size);
}

void __cdecl __free__(void * i_ptr)
{
	MemorySystem::Free (i_ptr);
}

void * operator new(size_t i_size)
{
	return MemorySystem::Alloc (i_size);
}

void operator delete(void * i_ptr)
{
	MemorySystem::Free (i_ptr);
}

void * operator new[](size_t i_size)
{
	return MemorySystem::Alloc (i_size);
}

void operator delete [](void * i_ptr)
{
	MemorySystem::Free (i_ptr);
}