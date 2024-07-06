#ifndef CPU_HPP
#define CPU_HPP
#pragma once

#include "Defines.hpp"
#include "GeneralPurposeRegister.hpp"
#include <cstdint>
#include <intrin.h>



#pragma pack(push, 1)
struct _DESCRIPTOR_REGISTER
{
    uint16_t limit;
    uintmax_t base;
};

#pragma pack (pop)

struct _SEGMENT_DESCRIPTOR
{
    union
    {
        uintmax_t value;
        struct
        {
            uint16_t limitLow;        // [0:15]
            uint16_t baseLow;         // [16:31]
            uint32_t baseMiddle : 8;  // [32:39]
            uint32_t type : 4;        // [40:43]
            uint32_t system : 1;      // [44]
            uint32_t dpl : 2;         // [45:46]
            uint32_t present : 1;     // [47]
            uint32_t limitHigh : 4;   // [48:51]
            uint32_t avl : 1;         // [52]
            uint32_t longMode : 1;    // [53]
            uint32_t defaultBit : 1;  // [54]
            uint32_t granularity : 1; // [55]
            uint32_t baseHigh : 8;    // [56:63]
        }bit;
    };
};


struct _SEGMENT_ATTRIBUTES
{
    union
    {
        uint16_t value;
        struct
        {
            uint16_t type : 4;        // [0:3]
            uint16_t system : 1;      // [4]
            uint16_t dpl : 2;         // [5:6]
            uint16_t present : 1;     // [7]
            uint16_t avl : 1;         // [8]
            uint16_t longMode : 1;    // [9]
            uint16_t defaultBit : 1;  // [10]
            uint16_t granularity : 1; // [11]
            uint16_t reserved0 : 4;   // [12:15]
        }bit;
    };
};


struct _SEGMENT_SELECTOR
{
    union
    {
        uint16_t value;
        struct
        {
            uint16_t rpl : 2;         // [0:1]
            uint16_t ti : 1;          // [2]
            uint16_t index : 13;      // [3:15]
        }bit;
    };
};


struct _SEGMENT_REGISTER
{
    _SEGMENT_SELECTOR selector;
    _SEGMENT_ATTRIBUTES attributes;
    uint32_t limit;
    uintmax_t base;

    void FetchAtts(__in uintmax_t gdtBase)
    {
        const auto descriptor = reinterpret_cast<_SEGMENT_DESCRIPTOR*>(gdtBase + (selector.value &~ 0x3));
        
        attributes.bit.type = descriptor->bit.type;
        attributes.bit.system = descriptor->bit.system;
        attributes.bit.dpl = descriptor->bit.dpl;
        attributes.bit.present = descriptor->bit.present;
        attributes.bit.avl = descriptor->bit.avl;
        attributes.bit.longMode = descriptor->bit.longMode;
        attributes.bit.defaultBit = descriptor->bit.defaultBit;
        attributes.bit.granularity = descriptor->bit.granularity;
        attributes.bit.present = descriptor->bit.present;
        attributes.bit.reserved0 = 0;
    }

};


struct _XCR0_REGISTER
{
private:
    union
    {
        uintmax_t value;
        struct
        {
            uintmax_t x87 : 1;
            uintmax_t sse : 1;
            uintmax_t ymm : 1;
            uintmax_t reserved0 : 6;
            uintmax_t mpk : 1;
            uintmax_t reserved1 : 1;
            uintmax_t cetu : 1;
            uintmax_t cets : 1;
            uintmax_t reserved2 : 49;
            uintmax_t lwp : 1;
            uintmax_t x : 1;
        }bit;
    };

public:

    __forceinline void WriteValue() const
    {
        _xsetbv(0x0, this->value);
    }


    __forceinline void ReadValue()
    {
        this->value = _xgetbv(0x0);
    }


    __forceinline auto GetValue() const
    {
        return this->value;
    }


    __forceinline auto CheckX87() const
    {
        return this->bit.x87 ? 1 : 0;
    }


    __forceinline auto operator=(__in const uintmax_t& val)
    {
        this->value = val;
    }

    __forceinline auto operator==(const _XCR0_REGISTER& other)
    {
       for (uint32_t i = 0; i < 10; i++)
	   {
		   if (((this->value >> i) & 1) != ((other.value >> i) & 1))
			   return false;
	   }

       return true;
    }

    __forceinline auto operator!=(const _XCR0_REGISTER& other)
    {
        for (uint32_t i = 0; i < 10; i++)
		{
			if (((this->value >> i) & 1) != ((other.value >> i) & 1))
				return true;
		}

        return false;
    }
};


struct _APIC_BAR
{
    union
    {
        uintmax_t value;
        struct
        {
            uintmax_t reserved0 : 8;
            uintmax_t bootstrapCpuCore : 1;
            uintmax_t reserved1 : 1;
            uintmax_t apicx2Enable : 1;
            uintmax_t apicEnable : 1;
            uintmax_t apicBase : 40;
            uintmax_t reserved2 : 12;
        }bit;
    };
};


struct _GUEST_REGISTERS
{
    _GP_REGISTER rax;
    _GP_REGISTER rcx;
    _GP_REGISTER rdx;
    _GP_REGISTER rbx;
    _GP_REGISTER rsp;
    _GP_REGISTER rbp;
    _GP_REGISTER rsi;
    _GP_REGISTER rdi;
    _GP_REGISTER r8;
    _GP_REGISTER r9;
    _GP_REGISTER r10;
    _GP_REGISTER r11;
    _GP_REGISTER r12;
    _GP_REGISTER r13;
    _GP_REGISTER r14;
    _GP_REGISTER r15;

    _XMM_REGISTER xmm15;
    _XMM_REGISTER xmm14;
    _XMM_REGISTER xmm13;
    _XMM_REGISTER xmm12;
    _XMM_REGISTER xmm11;
    _XMM_REGISTER xmm10;
    _XMM_REGISTER xmm9;
    _XMM_REGISTER xmm8;
    _XMM_REGISTER xmm7;
    _XMM_REGISTER xmm6;
    _XMM_REGISTER xmm5;
    _XMM_REGISTER xmm4;
    _XMM_REGISTER xmm3;
    _XMM_REGISTER xmm2;
    _XMM_REGISTER xmm1;
    _XMM_REGISTER xmm0; 
};



#endif // !CPU_HXX

