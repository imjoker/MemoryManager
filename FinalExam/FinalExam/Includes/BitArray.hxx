#pragma once

#include "BitArray.hpp"

// init
inline
void
BitArray::Init (UInt32 pCapacity, HeapManager * pHM, bool pInitVal) noexcept
{
    assert (!(pCapacity == 0 || pCapacity > MAX_MULTIPLE_OF_8_TUINT32));

    vCapacity   = GET_MULTIPLE_OF (pCapacity, NUM_BITS_PER_INDEX);
    vArraySize  = GetArraySize (vCapacity);
    vArray      = pHM->Alloc<BitData>(vArraySize);

    ReInitializeBitVector (pInitVal);
}

// Destructor
inline
void
BitArray::Destroy (HeapManager * pHM) noexcept
{
    pHM->Free (vArray);
}

/**
 * @brief  Reinitializes entire BitArray with the passed 
 *         pInitValue.
 *  
 * @param  pInitVal : value with which the data in BitArray 
 *                    should be initialized with.
 */
inline
void BitArray::ReInitializeBitVector (bool pInitVal) noexcept
{
    if (!pInitVal) {

        memset (static_cast<void *>(vArray), 0, vArraySize);
    } else {

        memset (static_cast<void *>(vArray), BYTE_SET, vArraySize);
    }
}


/**
 * @brief   Sets the bit at passed index of BitArray. 
 * 
 *          Calculates the index at which the pIndex'th
 *          bit lie in vArray and does bitwise
 *          operation to set it. 
 *  
 * @param   pIndex : index at which data needs to be set  
 * 
 * @return  void
 */
inline
void BitArray::Set (UInt32 pIndex) noexcept
{    
    assert (pIndex <= vCapacity);

    /**
     * 
     * Elaboration of the operation we are performing in this interface if we consider an index to be just an 8-bit value
     * 
     * our macro values are:
     * BIT_MASK = 7
     * BIT_MASK = 8
     * 
     * and,
     * 
     * pIndex / 8   - gives us the index of vArray in which the data lies
     * pIndex & 7   - gives us the bit-index of the UInt8 byte located at (pIndex / 8)'th pos (bit-index)
     *  
     * if we assume "Int8 data" as the data inside a particular 
     * index in the "Int8 * vArray" where the desired bit lie. 
     * i.e. data = vArray [pIndex / 8]
     *  
     * then the below Bitwise operation is equivalent to
     *  
     *              data = data | (1 << (pIndex & 7));
     *  
     * As "pIndex & 7" gives us the 'bit-index' inside the 'data',
     * we first do left shift on 1 with this 'bit-index'.
     * Then, we perform "bitwise-or" operation on data with this 
     * left shifted value to set the desired bit.
     *  
     * Eg: suppose 'data' is -> 10001101 
     *     and (pIndex & 7) resulted as 4 (bit-index in 'data')
     *  
     *     1 << (4)  ->  00010000
     *  
     *  and the operation would result as following
     *  
     *     data     | (1 << (pIndex & 7))
     *     10001101 | 00010000              -> 10011101
     *        ^                                   ^
     *  
     * As a result, we see that the 4th bit is set. 
     *  
     */
    vArray[pIndex / NUM_BITS_PER_INDEX] |= (static_cast<BitData> (1) << (pIndex & BIT_MASK));
}


/**
 * @brief   Unsets the bit at the passed index of BitArray. 
 * 
 *          Calculates the index at which the pIndex'th
 *          bit lie in vArray and does bitwise
 *          operation to unset it.  
 *  
 * @param   pIndex : index at which data needs to be unset  
 * 
 * @return  void
 */
inline
void BitArray::Unset (UInt32 pIndex) noexcept
{
    assert (pIndex <= vCapacity);

    /**
     *
     * Elaboration of the operation we are performing in this interface if we consider an index to be just an 8-bit value
     *
     * our macro values are:
     * BIT_MASK = 7
     * BIT_MASK = 8
     *
     * and,
     *
     * pIndex / 8   - gives us the index of vArray in which the data lies
     * pIndex & 7   - gives us the bit-index of the UInt8 byte located at (pIndex / 8)'th pos (bit-index)
     *
     * if we assume "Int8 data" as the data inside a particular
     * index in the "Int8 * vArray" where the desired bit lie.
     * i.e. data = vArray [pIndex / 8]
     *
     * then the below Bitwise operation is equivalent to
     *
     *              data = data & ~(1 << (pIndex & 7);
     *
     * As "pIndex & 7" gives us the 'bit-index' inside the 'data', we first
     * do left shift on 1 with this 'bit-index' and negated the result.
     * Then, we perform "bitwise-and" operation on data with this
     * negated value to unset the desired bit.
     *
     * Eg: suppose data -> 10011101
     *     and (pIndex & 7) resulted as 4 (bit-index in 'data')
     *
     *     ~(1 << 4)  -> 11101111
     *
     *  and the operation would result as following
     *
     *     data     & ~(1 << (pIndex & 7))
     *     10011101 & 11101111              -> 10001101
     *        ^                                   ^
     *
     * As a result, we see that the 4th bit is unset.
     *
     */
    vArray[pIndex / NUM_BITS_PER_INDEX] &= (~(static_cast<BitData> (1) << (pIndex & BIT_MASK)));
}

inline 
void BitArray::SetAll() noexcept
{
    ReInitializeBitVector (true);
}

inline
void BitArray::UnsetAll() noexcept
{
    ReInitializeBitVector (false);
}


/**
 * @brief   Returns the bit-value at passed index of BitArray. 
 * 
 *          Calculates the index at which the pIndex'th
 *          bit lie in vArray, loads the data, and returns
 *          the bit-value at the specified index. 
 *  
 * @param   pIndex : index from which data needs to read  
 * 
 * @return  bool 
 */
inline
bool BitArray::GetAt (UInt32 pIndex) const noexcept
{
    assert (pIndex <= vCapacity);

         /**
         * Elaboration of the operation we are performing in this interface if we consider an index to be just an 8-bit value
         *
         * our macro values are:
         * BIT_MASK = 7
         * BIT_MASK = 8
         *
         * and,
         *
         * pIndex / 8   - gives us the index of vArray in which the data lies
         * pIndex & 7   - gives us the bit-index of the UInt8 byte located at (pIndex / 8)'th pos (bit-index)
         * 
         * Illustration with an example:
         * 
         *  suppose data -> 10011101
         *  and (pIndex & 7) resulted as 4 (bit-index in data)
         *  
         *     1 << (4)  ->  00010000
         *  
         *  and the operation would result as following
         *  
         *      data      &  (1 << (pIndex & 8))
         *      10011101  &  00010000             -> 00010000
         *         ^          ^                         ^
         *  
         *  as the bit is set, UInt8 value of the result would be 16,
         *  which is "1/true" when returned as bool.
         *  
         */
        BitData data = vArray[pIndex / NUM_BITS_PER_INDEX];

    return (data & (static_cast<BitData> (1) << (pIndex & BIT_MASK)));
}


/**
 * @brief  returns the value at passed index of BitArray
 *  
 * @param  pIndex : index from which data needs to read  
 * 
 * @return bool 
 */
inline
bool BitArray::operator [] (UInt32 pIndex) const noexcept
{
    return GetAt (pIndex);
}


/**
 * @brief  returns capacity of the BitArray
 *         in the units of bits.
 *
 * @return UInt32
 */
inline
UInt32 BitArray::GetCapacity () const noexcept
{
    return vCapacity;
}

/**
* @brief  returns true if the bit at pIndex is Set. Else, false.
*
* @return bool
*/
inline 
bool BitArray::IsBitSet (UInt32 pIndex) const noexcept
{
    return (GetAt (pIndex) == true);
}

/**
* @brief  returns true if the bit at pIndex is Unset. Else, false.
*
* @return bool
*/
inline
bool BitArray::IsBitUnset (UInt32 pIndex) const noexcept
{
    return (GetAt (pIndex) == false);
}

inline 
bool BitArray::AreAllBitsSet() const noexcept
{
        UInt32 index;

    return (!FindFirstUnsetBit (index));
}

/**
* @brief  finds the index of the first
*         set bit in the bitarray.
*
* @return bool: Returns true, if a set bit is found. Else, False.
*/
inline
bool
BitArray::FindFirstSetBit (UInt32 & pFirstSetBitIndex) const noexcept
{
        UInt32 index = 0;
        bool   isFound;

    while ((vArray[index] == BitData(0)) && (index < vArraySize - 1))
        ++index;

#if(_WIN64)
    isFound = BitScanForward64 ((unsigned long *)&pFirstSetBitIndex, vArray[index]);
#else
    isFound = _BitScanForward ((unsigned long *)&pFirstSetBitIndex, vArray[index]);
#endif

    pFirstSetBitIndex += (index * NUM_BITS_PER_INDEX);

    return isFound;
}

/**
* @brief  finds the index of the first
*         unset bit in the bitarray.
*
* @return bool: Returns true, if an unset bit is found. Else, False.
*/
inline
bool
BitArray::FindFirstUnsetBit (UInt32 & pFirstSetBitIndex) const noexcept
{
        UInt32 index = 0;
        BitData var;
        bool   isFound;

    do {

        var = ~(vArray[index]);  // negate the value so that we can use inbuilt function to scan bits.

#if(_WIN64)
        isFound = _BitScanForward64 ((unsigned long *)&pFirstSetBitIndex, var);
#else
        isFound = _BitScanForward ((unsigned long *)&pFirstSetBitIndex, var);
#endif    

        if (isFound) {
        
            pFirstSetBitIndex += (index * NUM_BITS_PER_INDEX);

            return isFound;
        }

        ++index;

    } while (index < vArraySize);   // loop till last index.

    return false;
}

/**
 * @brief  calculates the array size (in bytes) that 
 *         would be needed to provide pCapacity bits.
 * 
 * @param  pCapacity : no. of bits that user has requested 
 *
 * @return UInt32
 */
inline
UInt32
BitArray::GetArraySize (UInt32 pCapacity) noexcept
{
    return (pCapacity / NUM_BITS_PER_INDEX);
}

