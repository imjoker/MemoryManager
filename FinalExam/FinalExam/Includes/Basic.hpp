#pragma once

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <intrin.h>

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>           
#include <cstddef>

#pragma intrinsic(_BitScanForward)

using UInt8   =  unsigned _int8;
using UInt16  =  unsigned _int16;
using UInt32  =  unsigned _int32;
using UInt64  =  unsigned _int64;
using Int8    =  signed _int8;
using Int16   =  signed _int16;
using Int32   =  signed _int32;
using Int64   =  signed _int64;

#if(_WIN64)
using BitData =  UInt64;
using Int     =  Int64;
#else
using BitData =  UInt32;
using Int     =  Int32;
#endif

using Byte = char;

using namespace std;

#define GET_MULTIPLE_OF(pValue, pFactor) (((pValue + pFactor - 1U) / pFactor) * pFactor);