#pragma once

#include "HeapManager.hpp"

class BitArray
{
public:

    /* init */
    inline void                 Init                        (UInt32 pCapacity, HeapManager * pHM, bool pInitVal = false) noexcept;

    /* destructor */
    inline  void                Destroy                     (HeapManager * pHM) noexcept;

    /* public modifiers */

    inline  void                Set                         (UInt32 pIndex) noexcept; 
    inline  void                Unset                       (UInt32 pIndex) noexcept;

    inline  void                SetAll                      () noexcept; 
    inline  void                UnsetAll                    () noexcept;

    /* public getters */
    inline  bool                GetAt                       (UInt32 pIndex) const noexcept;         
    inline  bool                operator []                 (UInt32 pIndex) const noexcept;    
    inline  UInt32              GetCapacity                 () const noexcept;

    /* Queries */
    inline  bool                IsBitSet                    (UInt32 pIndex) const noexcept;
    inline  bool                IsBitUnset                  (UInt32 pIndex) const noexcept;

    inline  bool                AreAllBitsSet               () const noexcept;

    /* find */
    inline  bool                FindFirstSetBit            (UInt32 & pFirstSetBitIndex) const noexcept;
    inline  bool                FindFirstUnsetBit          (UInt32 & pFirstSetBitIndex) const noexcept;

private:

    /* private helper functions */
static inline  UInt32           GetArraySize                (UInt32 pCapacity) noexcept;
       inline  void             ReInitializeBitVector       (bool pInitVal = false) noexcept;

    /* private data members */
    UInt32             vCapacity;                  ///< capacity of array. tells us how many bits it can hold.
    UInt32             vArraySize;                 ///< size of vArray in bytes.
    BitData *          vArray;                     ///< zero initialized array in which data lies. each index of the array will have 64(for Win64)/32 (for Win32) elements (As the element here itself is a bit).         

    static constexpr UInt64  MAX_MULTIPLE_OF_8_TUINT64 = ((UInt64) 0xFFFF'FFFF'FFFF'FFF8);
    static constexpr UInt32  MAX_MULTIPLE_OF_8_TUINT32 = ((UInt32) 0xFFFF'FFF8);

    static constexpr UInt8   BYTE_SET                  = 0xFF;

#if (_WIN64)
    static constexpr UInt8   NUM_BITS_PER_INDEX        = 64;
    static constexpr UInt8   BIT_MASK                  = 63;
#else
    static constexpr UInt8   NUM_BITS_PER_INDEX        = 32;
    static constexpr UInt8   BIT_MASK                  = 31;
#endif
};

#include "BitArray.hxx"
