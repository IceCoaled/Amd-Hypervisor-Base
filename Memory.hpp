#ifndef MEMORY_HPP
#define MEMORY_HPP


#define PAGE_SIZE               0x1000
#define PAGE_TABLE_SIZE         0x200

#pragma once
#include <cstdint>

#pragma pack(push, 1)
union _IA32_ADDRESS_TRANSLATOR
{

    uintmax_t value;
    struct
    {
        uintmax_t reserved0 : 12;
        uintmax_t pt : 12;
        uintmax_t pd : 12;
        uintmax_t pdpt : 12;
        uintmax_t pml4 : 12;
    }asIndex;

    union
    {
        uintmax_t mapping4Kb : 12;
        uintmax_t mapping2Mb : 21;
        uintmax_t mapping1Gb : 30;
    }asPageOffset;
};
#pragma pack(pop)

struct _VIRTUAL_ADDRESS
{
    union
    {
        uintmax_t value;
        struct
        {
            uintmax_t offset : 12;
            uintmax_t pt : 9;
            uintmax_t pd : 9;
            uintmax_t pdpt : 9;
            uintmax_t pml4 : 9;
            uintmax_t reserved1 : 16;
        }Bit;
    };
};



struct _PT_ENTRY_4KB
{
    union
    {
        uintmax_t value;
        struct
        {
            uintmax_t valid : 1;               // [0]
            uintmax_t write : 1;               // [1]
            uintmax_t user : 1;                // [2]
            uintmax_t writeThrough : 1;        // [3]
            uintmax_t cacheDisable : 1;        // [4]
            uintmax_t accessed : 1;            // [5]
            uintmax_t dirty : 1;               // [6]
            uintmax_t pat : 1;                 // [7]
            uintmax_t global : 1;              // [8]
            uintmax_t avl : 3;                 // [9:11]
            uintmax_t pageFrameNumber : 40;    // [12:51]
            uintmax_t reserved1 : 11;          // [52:62]
            uintmax_t noExecute : 1;           // [63]
        }bit;
    };
};

struct _PML4_ENTRY_2MB
{
    union
    {
        uintmax_t value;
        struct
        {
            uintmax_t valid : 1;               // [0]
            uintmax_t write : 1;               // [1]
            uintmax_t user : 1;                // [2]
            uintmax_t writeThrough : 1;        // [3]
            uintmax_t cacheDisable : 1;        // [4]
            uintmax_t accessed : 1;            // [5]
            uintmax_t reserved0 : 3;           // [6:8]
            uintmax_t avl : 3;                 // [9:11]
            uintmax_t pageFrameNumber : 40;    // [12:51]
            uintmax_t reserved1 : 11;          // [52:62]
            uintmax_t noExecute : 1;           // [63]
        }bit;
    };
};
using _PDP_ENTRY_2MB = _PML4_ENTRY_2MB;

struct _PD_ENTRY_2MB
{
    union
    {
        uintmax_t value;
        struct
        {
            uintmax_t valid : 1;               // [0]
            uintmax_t write : 1;               // [1]
            uintmax_t user : 1;                // [2]
            uintmax_t writeThrough : 1;        // [3]
            uintmax_t cacheDisable : 1;        // [4]
            uintmax_t accessed : 1;            // [5]
            uintmax_t dirty : 1;               // [6]
            uintmax_t largePage : 1;           // [7]
            uintmax_t global : 1;              // [8]
            uintmax_t avl : 3;                 // [9:11]
            uintmax_t pat : 1;                 // [12]
            uintmax_t reserved0 : 8;           // [13:20]
            uintmax_t pageFrameNumber : 31;    // [21:51]
            uintmax_t reserved1 : 11;          // [52:62]
            uintmax_t noExecute : 1;           // [63]
        }bit;
    };
};


struct _SHARED_PAGES
{
    __declspec(align(PAGE_SIZE)) _PML4_ENTRY_2MB pml4e[PAGE_SIZE];
    __declspec(align(PAGE_SIZE)) _PDP_ENTRY_2MB pdpe[TABLE_PAGE_SIZE];
    __declspec(align(PAGE_SIZE)) _PD_ENTRY_2MB pde[TABLE_PAGE_SIZE][TABLE_PAGE_SIZE];
};


#endif // !MEMORY_HPP