#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include "Defines.hpp"

/**
 * @brief Represents an IA-32 address translator.
 *
 * This structure is used to translate linear addresses into different page table components,
 * including Page Table (PT), Page Directory (PD), Page Directory Pointer Table (PDPT), and Page Map Level 4 (PML4).
 */
struct _IA32_ADDRESS_TRANSLATOR
{
    union
    {
        uintmax_t value; ///< The full value of the address translator
        struct
        {
            uintmax_t reserved0 : 12; ///< Reserved bits
            uintmax_t pt : 12; ///< Page Table index
            uintmax_t pd : 12; ///< Page Directory index
            uintmax_t pdpt : 12; ///< Page Directory Pointer Table index
            uintmax_t pml4 : 12; ///< Page Map Level 4 index
        } asIndex;

        union
        {
            uintmax_t mapping4Kb : 12; ///< Page offset for 4KB pages
            uintmax_t mapping2Mb : 21; ///< Page offset for 2MB pages
            uintmax_t mapping1Gb : 30; ///< Page offset for 1GB pages
        } asPageOffset;
    };
};

/**
 * @brief Represents a virtual address.
 */
struct _VIRTUAL_ADDRESS
{
    union
    {
        uintmax_t value; ///< The full value of the virtual address
        struct
        {
            uintmax_t offset : 12; ///< Offset within the page
            uintmax_t pt : 9; ///< Page Table index
            uintmax_t pd : 9; ///< Page Directory index
            uintmax_t pdpt : 9; ///< Page Directory Pointer Table index
            uintmax_t pml4 : 9; ///< Page Map Level 4 index
            uintmax_t reserved1 : 16; ///< Reserved bits
        } bit;
    };
};

/**
 * @brief Represents a Page Table entry for 4KB pages.
 */
struct _PT_ENTRY_4KB
{
    union
    {
        uintmax_t value; ///< The full value of the page table entry
        struct
        {
            uintmax_t valid : 1; ///< Page is present in memory [0]
            uintmax_t write : 1; ///< Page is writable [1]
            uintmax_t user : 1; ///< User-mode access is allowed [2]
            uintmax_t writeThrough : 1; ///< Write-through caching is enabled [3]
            uintmax_t cacheDisable : 1; ///< Caching is disabled [4]
            uintmax_t accessed : 1; ///< Page has been accessed [5]
            uintmax_t dirty : 1; ///< Page has been written to [6]
            uintmax_t pat : 1; ///< Page Attribute Table index [7]
            uintmax_t global : 1; ///< Global page flag [8]
            uintmax_t avl : 3; ///< Available for system use [9:11]
            uintmax_t pageFrameNumber : 40; ///< Page frame number [12:51]
            uintmax_t reserved1 : 11; ///< Reserved bits [52:62]
            uintmax_t noExecute : 1; ///< No execute flag [63]
        } bit;
    };
};

/**
 * @brief Represents a Page Map Level 4 entry for 2MB pages.
 */
struct _PML4_ENTRY_2MB
{
    union
    {
        uintmax_t value; ///< The full value of the PML4 entry
        struct
        {
            uintmax_t valid : 1; ///< Page is present in memory [0]
            uintmax_t write : 1; ///< Page is writable [1]
            uintmax_t user : 1; ///< User-mode access is allowed [2]
            uintmax_t writeThrough : 1; ///< Write-through caching is enabled [3]
            uintmax_t cacheDisable : 1; ///< Caching is disabled [4]
            uintmax_t accessed : 1; ///< Page has been accessed [5]
            uintmax_t reserved0 : 3; ///< Reserved bits [6:8]
            uintmax_t avl : 3; ///< Available for system use [9:11]
            uintmax_t pageFrameNumber : 40; ///< Page frame number [12:51]
            uintmax_t reserved1 : 11; ///< Reserved bits [52:62]
            uintmax_t noExecute : 1; ///< No execute flag [63]
        } bit;
    };
};
using _PDP_ENTRY_2MB = _PML4_ENTRY_2MB; ///< PDP entry is identical to PML4 entry for 2MB pages

/**
 * @brief Represents a Page Directory entry for 2MB pages.
 */
struct _PD_ENTRY_2MB
{
    union
    {
        uintmax_t value; ///< The full value of the page directory entry
        struct
        {
            uintmax_t valid : 1; ///< Page is present in memory [0]
            uintmax_t write : 1; ///< Page is writable [1]
            uintmax_t user : 1; ///< User-mode access is allowed [2]
            uintmax_t writeThrough : 1; ///< Write-through caching is enabled [3]
            uintmax_t cacheDisable : 1; ///< Caching is disabled [4]
            uintmax_t accessed : 1; ///< Page has been accessed [5]
            uintmax_t dirty : 1; ///< Page has been written to [6]
            uintmax_t largePage : 1; ///< Indicates a large 2MB page [7]
            uintmax_t global : 1; ///< Global page flag [8]
            uintmax_t avl : 3; ///< Available for system use [9:11]
            uintmax_t pat : 1; ///< Page Attribute Table index [12]
            uintmax_t reserved0 : 8; ///< Reserved bits [13:20]
            uintmax_t pageFrameNumber : 31; ///< Page frame number [21:51]
            uintmax_t reserved1 : 11; ///< Reserved bits [52:62]
            uintmax_t noExecute : 1; ///< No execute flag [63]
        } bit;
    };
};

/**
 * @brief Represents shared pages used for MSR mapping and page tables.
 */
struct _SHARED_PAGES
{
    void* msrMap; ///< Pointer to MSR map
    __declspec( align( PAGE_SIZE ) ) _PML4_ENTRY_2MB pml4e[ PAGE_SIZE ]; ///< PML4 entries
    __declspec( align( PAGE_SIZE ) ) _PDP_ENTRY_2MB pdpe[ TABLE_PAGE_SIZE ]; ///< PDPT entries
    __declspec( align( PAGE_SIZE ) ) _PD_ENTRY_2MB pde[ TABLE_PAGE_SIZE ][ TABLE_PAGE_SIZE ]; ///< Page Directory entries
};

#endif // !MEMORY_HPP
