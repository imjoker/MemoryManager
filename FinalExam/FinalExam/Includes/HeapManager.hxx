#pragma once

inline
bool
HeapManager::IsPowerOfTwo (unsigned __int64 pValue) noexcept
{
	return (!(pValue & (pValue - 1)) && pValue);
}

inline
size_t
HeapManager::AlignDown (size_t pValue, BitData pAlign) noexcept
{
	assert (pAlign);
	assert (IsPowerOfTwo(pAlign));

	return (pValue & (~(pAlign - 1)));
}

inline
size_t
HeapManager::AlignUp (size_t pValue, BitData pAlign) noexcept
{
	assert (pAlign);
	assert (IsPowerOfTwo(pAlign));

	return (((pValue + pAlign - 1U) / pAlign) * pAlign);
}