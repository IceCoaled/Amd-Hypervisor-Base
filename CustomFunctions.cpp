#include "CustomFunctions.hpp"

#include <ntddk.h>


/**
 * @brief The Inlines namespace contains a collection of custom utility functions.
 *
 * Many functions within this namespace are inlined for performance reasons. These functions
 * provide a variety of operations such as string manipulation, memory management, signature
 * scanning, and module handling.
 *
 * @remarks
 * The functions in the Inlines namespace are designed for low-level system tasks and are
 * optimized for speed through inlining. These functions include, but are not limited to,
 * memory allocation, memory comparison, and address translation. The namespace also provides
 * functionality for working with system modules, such as retrieving and searching through
 * loaded module lists.
 */
namespace Inlines
{

    /**
     * @brief Calculates the length of a given string.
     *
     * @tparam T The type of the string (e.g., char*).
     * @param string The input string whose length is to be determined.
     * @return The length of the input string.
     */
    template <typename T>
    size_t strLen( __inout T string )
    {
        T counter{};

        for ( counter = string; *counter; ++counter );

        return ( counter - string );
    }


    /**
     * @brief Converts a character string to an integer.
     *
     * @param string The character string to be converted.
     * @return The integer value represented by the string.
     */
    uintmax_t CharToInt( __inout char* string )
    {
        uintmax_t result = 0;

        while ( *string )
        {
            if ( *string <= '0' || *string >= '9' ) return 0;

            result *= 10;
            result += *string - '0';
            ++string;
        }

        return result;
    }


    /**
     * @brief Converts a string to lowercase.
     *
     * @param string The input string to be converted.
     * @param len The length of the string.
     */
    void lowerCase( __in char* string, __in size_t len )
    {
        while ( len-- > 0 )
        {
            *string += ( ( unsigned char )( *string - 'A' ) < 26 ) << 5;
            ++string;
        }
    }


    /**
     * @brief Compares two memory blocks for equality.
     *
     * @param lhs Pointer to the first memory block.
     * @param rhs Pointer to the second memory block.
     * @param length The number of bytes to compare.
     * @return 0 if memory blocks are equal, 1 otherwise.
     */
    int memCpr( __inout void* lhs, __inout void* rhs, __in size_t length )
    {
        size_t it = 0;

        while ( it < length )
        {
            if ( ( ( ( volatile uint8_t* )lhs )[ it ] - ( ( volatile uint8_t* )rhs )[ it ] ) != 0 ) return 1;
            it++;
        }

        return 0;
    }


    /**
     * @brief Sets a memory block to a specified value.
     *
     * @param mem Pointer to the memory block.
     * @param setValue The value to set.
     * @param length The length of the memory block.
     */
    void memSet( __inout void* mem, __in const uint8_t setValue, __in size_t length )
    {
        size_t it = 0;

        while ( it < length )
        {
            ( ( volatile uint8_t* )mem )[ it ] = setValue;
            it++;
        }
    }


    /**
     * @brief Copies a memory block from source to destination.
     *
     * @param dest Pointer to the destination memory block.
     * @param src Pointer to the source memory block.
     * @param length The number of bytes to copy.
     */
    void memCpy( __inout void* dest, __inout void* src, __in size_t length )
    {
        size_t it = 0;

        while ( it < length )
        {
            ( ( uint8_t* )dest )[ it ] = ( ( uint8_t* )src )[ it ];

            it++;
        }
    }


    /**
     * @brief Compares two strings for equality.
     *
     * @tparam S1 The type of the first string.
     * @tparam S2 The type of the second string.
     * @param string1 The first string.
     * @param string2 The second string.
     * @return 0 if strings are equal, non-zero otherwise.
     */
    template <typename S1, typename S2>
    int strCmp( __inout S1 string1, __inout S2 string2 )
    {
        while ( *string1 && *string1 == *string2 )
        {
            ++string1;
            ++string2;
        }
        return ( ( int )( ( uint8_t )*string1 ) - ( int )( ( uint8_t )*string2 ) );
    }


    /**
     * @brief Allocates non-paged pool memory.
     *
     * @param length The length of memory to allocate.
     * @return Pointer to the allocated memory, or nullptr if allocation fails.
     */
    void* poolAlloc( __in size_t length )
    {
        auto buffer = ExAllocatePool2( POOL_FLAG_NON_PAGED, length, 'MMsV' );
        if ( !buffer ) return nullptr;

        Inlines::memSet( buffer, 0, length );

        NT_VERIFY( PAGE_ALIGN( buffer ) == buffer );
        return buffer;
    }


    /**
     * @brief Frees non-paged pool memory.
     *
     * @param buffer Pointer to the memory to be freed.
     */
    void poolFree( __inout void* buffer )
    {
        ExFreePool( buffer );
    }


    /**
     * @brief Allocates contiguous physical memory.
     *
     * @param length The length of memory to allocate.
     * @return Pointer to the allocated memory, or nullptr if allocation fails.
     */
    void* contigAlloc( __in size_t length )
    {
        PHYSICAL_ADDRESS lowAddr = { 0 };
        PHYSICAL_ADDRESS highAddr = { 0 };
        highAddr.QuadPart = ~0x0;
        lowAddr.QuadPart = 0;

        auto buffer = MmAllocateContiguousMemorySpecifyCacheNode( length, lowAddr, highAddr, lowAddr, MmHardwareCoherentCached, MM_ANY_NODE_OK );
        if ( !buffer ) return nullptr;

        Inlines::memSet( buffer, 0, length );

        NT_VERIFY( PAGE_ALIGN( buffer ) == buffer );
        return buffer;
    }


    /**
     * @brief Frees contiguous physical memory.
     *
     * @param buffer Pointer to the memory to be freed.
     * @param szBuffer Size of the memory block to be freed.
     */
    void contigFree( __inout void* buffer, __in size_t szBuffer )
    {
        MmFreeContiguousMemorySpecifyCache( buffer, szBuffer, MmNonCached );
    }


    /**
     * @brief Scans a memory block for a specific pattern.
     *
     * @param memory Pointer to the memory block to be scanned.
     * @param length The length of the memory block.
     * @param pattern The pattern to search for.
     * @param patternLength The length of the pattern.
     * @return Pointer to the location of the pattern in memory, or nullptr if not found.
     */
    void* signatureScanner( __inout UCHAR* memory, __in size_t length, __inout UCHAR* pattern, __in size_t patternLength )
    {
        for ( size_t len = 0; len < length; len++ )
        {
            if ( memory[ len ] == pattern[ 0 ] &&
                 Inlines::memCpr( &memory[ len ], pattern, patternLength ) == 0 )
            {
                return &memory[ len ];
            }
        }

        return nullptr;
    }


    /**
     * @brief Retrieves the virtual page table entry (PTE) for a given system address.
     *
     * @param systemAddr The system address to get the PTE for.
     * @return Pointer to the virtual PTE.
     */
    void* getVirtualPte( __in uintmax_t systemAddr )
    {
        constexpr auto szMiGetPteAddress = 0x00013;

        auto pteBaseAddr = ( ( ( uintmax_t )Inlines::miGetPteAddress ) + szMiGetPteAddress );

        systemAddr = systemAddr >> 9;
        systemAddr &= 0x7FFFFFFFF8;
        systemAddr += pteBaseAddr;

        return ( void* )systemAddr;
    }


    /**
     * @brief Retrieves the physical page table entry (PTE) for a given physical address.
     *
     * @param physicalAddr The physical address to get the PTE for.
     * @return Pointer to the physical PTE.
     */
    void* getPhysicalPte( __in uintmax_t physicalAddr )
    {
        PHYSICAL_ADDRESS physAddr = { 0 };
        physAddr.QuadPart = physicalAddr;

        auto virtAddr = MmGetVirtualForPhysical( physAddr );

        return Inlines::getVirtualPte( ( uintmax_t )virtAddr );
    }


    /**
     * @brief Retrieves the list of system modules.
     *
     * @param modulelist Pointer to the system module information structure.
     * @return NTSTATUS indicating the success or failure of the operation.
     */
    NTSTATUS GetModuleList( __inout SYSTEM_MODULE_INFORMATION** modulelist )
    {
        NTSTATUS status = -1;
        unsigned long szBuffer = 0;

        do
        {
            status = ZwQuerySystemInformation( _SYSTEM_INFORMATION_CLASS::SystemModuleInformation, 0, szBuffer, &szBuffer );

            *modulelist = ( SYSTEM_MODULE_INFORMATION* )Inlines::poolAlloc( szBuffer );
            if ( *modulelist )
            {
                status = ZwQuerySystemInformation( _SYSTEM_INFORMATION_CLASS::SystemModuleInformation, *modulelist, szBuffer, &szBuffer );
            } else
            {
                return STATUS_UNSUCCESSFUL;
            }
        } while ( status == STATUS_INFO_LENGTH_MISMATCH );

        return STATUS_SUCCESS;
    }


    /**
     * @brief Finds a specific module in the system module list.
     *
     * @param modulelist Pointer to the system module information structure.
     * @param szModule Pointer to store the size of the module found.
     * @param moduleToFind The name of the module to find.
     * @return Pointer to the base address of the module, or nullptr if not found.
     */
    void* FindModule( __inout SYSTEM_MODULE_INFORMATION** modulelist, __inout size_t* szModule, const char* moduleToFind )
    {
        void* baseAddr = nullptr;
        size_t szImage = 0;

        SYSTEM_MODULE_ENTRY* p_me = ( *modulelist )->module;

        for ( ULONG i = 0; i < ( *modulelist )->count; i++ )
        {
            auto moduleName = ( ( char* )( p_me[ i ].FullPathName + p_me[ i ].OffsetToFileName ) );
            Inlines::lowerCase( moduleName, Inlines::strLen( moduleName ) );

            if ( Inlines::strCmp( const_cast< const char* >( moduleName ), moduleToFind ) == 0 )
            {
                baseAddr = p_me[ i ].ImageBase;
                szImage = p_me[ i ].ImageSize;
                break;
            }
        }

        if ( !baseAddr || !szImage )
        {
            return nullptr;
        }

        *szModule = szImage;

        return baseAddr;
    }


}; // !namespace Inlines
