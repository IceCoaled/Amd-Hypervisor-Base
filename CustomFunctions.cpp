#include "CustomFunctions.hpp"

#include <ntddk.h>


namespace Inlines
{

    template <typename T>
    size_t strLen(__in T string)
    {
        
        T counter{};

        for (counter = string; *counter; ++counter);

        return (counter - string);   
    }



    uintmax_t CharToInt(__inout char* string)
    {
        uintmax_t result = 0;

        while (*string)
        {
            if (*string <= '0' || *string >= '9') return 0;

            result *= 10;
            result += *string - '0';
            ++string;
        }

        return result;
    }



    void lowerCase(__in char* string, __in size_t len)
    {

        while (len-- > 0) {
            *string += ((unsigned char)(*string - 'A') < 26) << 5;
            ++string;
        }
    }



    int memCpr(__in void* lhs, __in void* rhs, __in size_t length)
    {
        size_t it = 0;

        while (it < length)
        {
            if ((((volatile uint8_t*)lhs)[it] - ((volatile uint8_t*)rhs)[it]) != 0) return 1;
            it++;
        }

        return 0;
    }




    void memSet(__in void* mem, __in uint8_t setValue, __in size_t length)
    {
        size_t it = 0;

        while (it < length)
        {
            ((volatile uint8_t*)mem)[it] = setValue;
            it++;
        }
    }




    void memCpy(__in void* dest, __in void* src, __in size_t length)
    {
        size_t it = 0;

        while (it < length)
        {
            ((uint8_t*)dest)[it] = ((uint8_t*)src)[it];

            it++;
        }
    }



    template <typename T>
    int strCmp(__in T string1, __in T string2)
    {

        while (*string1 && *string1 == *string2)
        {
            ++string1;
            ++string2;
        }
        return ((int)((uint8_t)*string1) - (int)((uint8_t)*string2));
    }



    void* poolAlloc(__in size_t length)
    {
        auto buffer = ExAllocatePool2(POOL_FLAG_NON_PAGED, length, 'MMsV');
        if (!buffer) return nullptr;

        Inlines::memSet(buffer, 0, length);

        NT_VERIFY(PAGE_ALIGN(buffer) == buffer);
        return buffer;
    }



    void poolFree(__in void* buffer)
    {
        ExFreePool(buffer);
    }



    void* contigAlloc(__in size_t length)
    {
        PHYSICAL_ADDRESS lowAddr = { 0 };
        PHYSICAL_ADDRESS highAddr = { 0 };
        highAddr.QuadPart = ~0x0;
        lowAddr.QuadPart = 0;

        auto buffer = MmAllocateContiguousMemorySpecifyCacheNode(length, lowAddr, highAddr, lowAddr, MmHardwareCoherentCached, MM_ANY_NODE_OK);
        if (!buffer) return nullptr;

        Inlines::memSet(buffer, 0, length);

        NT_VERIFY(PAGE_ALIGN(buffer) == buffer);
        return buffer;
    }



    void contigFree(__in void* buffer, __in size_t szBuffer)
    {
        MmFreeContiguousMemorySpecifyCache(buffer, szBuffer, MmNonCached);
    }




    void* signatureScanner(__in UCHAR* memory, __in size_t length, __in UCHAR* pattern, __in size_t patternLength)
    {

        for (size_t len = 0; len < length; len++)
        {
            if (memory[len] == pattern[0] && 
                Inlines::memCpr(&memory[len], pattern, patternLength) == 0)
            { 
                return &memory[len];
            }
        }

        return nullptr;
    }



    void* getVirtualPte(__in uintmax_t systemAddr)
    {
        constexpr auto szMiGetPteAddress = 0x00013;

        auto pteBaseAddr = (((uintmax_t)Inlines::miGetPteAddress) + szMiGetPteAddress);

        systemAddr = systemAddr >> 9;
        systemAddr &= 0x7FFFFFFFF8;
        systemAddr += pteBaseAddr;

        return (void*)systemAddr;
    }



    void* getPhysicalPte(__in uintmax_t physicalAddr)
    {
        PHYSICAL_ADDRESS physAddr = { 0 };
        physAddr.QuadPart = physicalAddr;

        auto virtAddr = MmGetVirtualForPhysical(physAddr);

        return Inlines::getVirtualPte((uintmax_t)virtAddr);
    }


    NTSTATUS GetModuleList(__inout SYSTEM_MODULE_INFORMATION** modulelist)
    {
        NTSTATUS status = -1;
        unsigned long szBuffer = 0;

        do
        {

            status = ZwQuerySystemInformation(_SYSTEM_INFORMATION_CLASS::SystemModuleInformation, 0, szBuffer, &szBuffer);


            *modulelist = (SYSTEM_MODULE_INFORMATION*)Inlines::poolAlloc(szBuffer);
            if (*modulelist)
            {

                status = ZwQuerySystemInformation(_SYSTEM_INFORMATION_CLASS::SystemModuleInformation, *modulelist, szBuffer, &szBuffer);
            }
            else
            {
                return STATUS_UNSUCCESSFUL;
            }


        } while (status == STATUS_INFO_LENGTH_MISMATCH);

        return STATUS_SUCCESS;
    }


    void* FindModule(__inout SYSTEM_MODULE_INFORMATION** modulelist, __inout size_t* szModule, const char* moduleToFind)
    {
        void* baseAddr = nullptr;
        size_t szImage = 0;

        SYSTEM_MODULE_ENTRY* p_me = (*modulelist)->module;

        for (ULONG i = 0; i < (*modulelist)->count; i++)
        {
            auto moduleName = ((char*)(p_me[i].FullPathName + p_me[i].OffsetToFileName));
            Inlines::lowerCase(moduleName, Inlines::strLen(moduleName));

            if (Inlines::strCmp(const_cast<const char*>(moduleName), moduleToFind) == 0)
            {
                baseAddr = p_me[i].ImageBase;
                szImage = p_me[i].ImageSize;
                break;
            }
        }


        if (!baseAddr || !szImage)
        {
            return nullptr;
        }


        *szModule = szImage;

        return baseAddr;
    }


}; // !namespace Inlines
