#pragma once

#include <iostream>
#include <stdio.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>       

#include "GLib.h"

using UInt8   =  unsigned _int8;
using UInt16  =  unsigned _int16;
using UInt32  =  unsigned _int32;
using UInt64  =  unsigned _int64;
using Int8    =  signed _int8;
using Int16   =  signed _int16;
using Int32   =  signed _int32;
using Int64   =  signed _int64;

using namespace std;

#define DISALLOW_COPY(ClassName)                    ClassName                (const ClassName &) = delete;         \
                                                    ClassName &  operator =  (const ClassName &) = delete;


#define DISALLOW_ON_STACK(ClassName)                ClassName                () = delete;                           \
                                                    ~ClassName               () = delete;

class MemMgr {

public: 

    static void * MALLOC (size_t pSize) noexcept;
};

inline
void *
MemMgr::MALLOC (size_t pSize) noexcept
{
    void * ptr;

    ptr = malloc (pSize);

    if (!ptr) {

        printf ("\nSystem ran out of memory\n");
        exit (0);       // application exits
    }

    return ptr;
}
