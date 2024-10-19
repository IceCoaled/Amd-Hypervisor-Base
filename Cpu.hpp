#ifndef CPU_HPP
#define CPU_HPP
#pragma once

#include "Defines.hpp"
#include "GeneralPurposeRegister.hpp"
#include <cstdint>
#include <intrin.h>



#pragma pack(push, 1)
/**
 * @brief Represents a descriptor register, including limit and base values.
 */
struct _DESCRIPTOR_REGISTER
{
    uint16_t limit; ///< Limit of the descriptor table
    uintmax_t base; ///< Base address of the descriptor table
};

#pragma pack (pop)

/**
 * @brief Represents a segment descriptor.
 */
struct _SEGMENT_DESCRIPTOR
{
    union
    {
        uintmax_t value; ///< The full value of the segment descriptor
        struct
        {
            uint16_t limitLow;        // [0:15] Lower 16 bits of the limit
            uint16_t baseLow;         // [16:31] Lower 16 bits of the base address
            uint32_t baseMiddle : 8;  // [32:39] Middle 8 bits of the base address
            uint32_t type : 4;        // [40:43] Type of the segment
            uint32_t system : 1;      // [44] Descriptor type (0 for system, 1 for code/data)
            uint32_t dpl : 2;         // [45:46] Descriptor Privilege Level
            uint32_t present : 1;     // [47] Segment present flag
            uint32_t limitHigh : 4;   // [48:51] Higher 4 bits of the limit
            uint32_t avl : 1;         // [52] Available for system use
            uint32_t longMode : 1;    // [53] Long mode flag
            uint32_t defaultBit : 1;  // [54] Default operation size (0 = 16-bit, 1 = 32-bit)
            uint32_t granularity : 1; // [55] Granularity flag (0 = byte, 1 = 4KB)
            uint32_t baseHigh : 8;    // [56:63] Upper 8 bits of the base address
        }bit;
    };
};

/**
 * @brief Represents the attributes of a segment.
 */
struct _SEGMENT_ATTRIBUTES
{
    union
    {
        uint16_t value; ///< The full value of the segment attributes
        struct
        {
            uint16_t type : 4;        // [0:3] Segment type
            uint16_t system : 1;      // [4] Descriptor type (0 for system, 1 for code/data)
            uint16_t dpl : 2;         // [5:6] Descriptor Privilege Level
            uint16_t present : 1;     // [7] Segment present flag
            uint16_t avl : 1;         // [8] Available for system use
            uint16_t longMode : 1;    // [9] Long mode flag
            uint16_t defaultBit : 1;  // [10] Default operation size (0 = 16-bit, 1 = 32-bit)
            uint16_t granularity : 1; // [11] Granularity flag (0 = byte, 1 = 4KB)
            uint16_t reserved0 : 4;   // [12:15] Reserved bits
        }bit;
    };
};

/**
 * @brief Represents a segment selector.
 */
struct _SEGMENT_SELECTOR
{
    union
    {
        uint16_t value; ///< The full value of the segment selector
        struct
        {
            uint16_t rpl : 2;         // [0:1] Requested Privilege Level
            uint16_t ti : 1;          // [2] Table Indicator (0 = GDT, 1 = LDT)
            uint16_t index : 13;      // [3:15] Index into the descriptor table
        }bit;
    };
};

/**
 * @brief Represents a segment register, including selector, attributes, limit, and base.
 */
struct _SEGMENT_REGISTER
{
    _SEGMENT_SELECTOR selector; ///< Segment selector
    _SEGMENT_ATTRIBUTES attributes; ///< Segment attributes
    uint32_t limit; ///< Segment limit
    uintmax_t base; ///< Segment base address

    /**
     * @brief Fetches the attributes from the GDT using the base address.
     *
     * @param gdtBase Base address of the Global Descriptor Table (GDT)
     */
    void FetchAtts( __in uintmax_t gdtBase )
    {
        const auto descriptor = reinterpret_cast< _SEGMENT_DESCRIPTOR* >( gdtBase + ( selector.value & ~0x3 ) );

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

/**
 * @brief Represents the XCR0 register, used for managing extended control registers.
 */
struct _XCR0_REGISTER
{
private:
    union
    {
        uintmax_t value; ///< The full value of the XCR0 register
        struct
        {
            uintmax_t x87 : 1; ///< x87 state management
            uintmax_t sse : 1; ///< SSE state management
            uintmax_t ymm : 1; ///< AVX (YMM) state management
            uintmax_t reserved0 : 6;
            uintmax_t mpk : 1; ///< Memory protection key support
            uintmax_t reserved1 : 1;
            uintmax_t cetu : 1; ///< CET user state
            uintmax_t cets : 1; ///< CET supervisor state
            uintmax_t reserved2 : 49;
            uintmax_t lwp : 1; ///< Last write protection flag
            uintmax_t x : 1;
        }bit;
    };

public:

    /**
     * @brief Writes the current value of the XCR0 register.
     */
    __forceinline void WriteValue() const
    {
        _xsetbv( 0x0, this->value );
    }

    /**
     * @brief Reads the value of the XCR0 register.
     */
    __forceinline void ReadValue()
    {
        this->value = _xgetbv( _XCR_XFEATURE_ENABLED_MASK );
    }

    /**
     * @brief Gets the value of the XCR0 register.
     *
     * @return The value of the XCR0 register
     */
    __forceinline auto GetValue() const
    {
        return this->value;
    }

    /**
     * @brief Checks if the x87 state is enabled.
     *
     * @return 1 if x87 is enabled, otherwise 0
     */
    __forceinline auto CheckX87() const
    {
        return this->bit.x87 ? 1 : 0;
    }

    /**
     * @brief Assigns a new value to the XCR0 register.
     *
     * @param val The new value to be assigned
     */
    __forceinline auto operator=( __in const uintmax_t& val )
    {
        this->value = val;
    }

    /**
     * @brief Compares two XCR0 registers for equality.
     *
     * @param other The other XCR0 register to compare against
     * @return True if the two registers are equal, otherwise false
     */
    __forceinline auto operator==( const _XCR0_REGISTER& other )
    {
        for ( uint32_t i = 0; i < 10; i++ )
        {
            if ( ( ( this->value >> i ) & 1 ) != ( ( other.value >> i ) & 1 ) )
                return false;
        }

        return true;
    }

    /**
     * @brief Compares two XCR0 registers for inequality.
     *
     * @param other The other XCR0 register to compare against
     * @return True if the two registers are not equal, otherwise false
     */
    __forceinline auto operator!=( const _XCR0_REGISTER& other )
    {
        for ( uint32_t i = 0; i < 10; i++ )
        {
            if ( ( ( this->value >> i ) & 1 ) != ( ( other.value >> i ) & 1 ) )
                return true;
        }

        return false;
    }
};


/**
 * @brief Represents the APIC Base Address Register (BAR).
 */
struct _APIC_BAR
{
    union
    {
        uintmax_t value; ///< The full value of the APIC BAR
        struct
        {
            uintmax_t reserved0 : 8; ///< Reserved bits [0:7]
            uintmax_t bootstrapCpuCore : 1; ///< Bootstrap processor flag
            uintmax_t reserved1 : 1; ///< Reserved bit [9]
            uintmax_t apicx2Enable : 1; ///< x2APIC enable flag
            uintmax_t apicEnable : 1; ///< APIC enable flag
            uintmax_t apicBase : 40; ///< Base address of the APIC
            uintmax_t reserved2 : 12; ///< Reserved bits [52:63]
        } bit;
    };
};

/**
 * @brief Represents the MSR_VM_CR register, used for managing VM control.
 */
struct _MSR_VM_CR
{
private:
    union
    {
        uintmax_t value; ///< The full value of the MSR_VM_CR register
        struct
        {
            uintmax_t dpd : 1; ///< Debug port disable flag
            uintmax_t rInit : 1; ///< Re-initialization flag
            uintmax_t disA20m : 1; ///< A20 masking disable flag
            uintmax_t svmLock : 1; ///< SVM lock flag
            uintmax_t svmeDisable : 1; ///< SVM enable disable flag
            uintmax_t reserved0 : 59; ///< Reserved bits
        } bit;
    };

public:
    /**
     * @brief Assigns a new value to the MSR_VM_CR register.
     *
     * @param val The new value to be assigned
     */
    __forceinline auto operator=( __in const uintmax_t& val )
    {
        this->value = val;
    }

    /**
     * @brief Checks if SVM lock is enabled.
     *
     * @return True if SVM lock is enabled, otherwise false
     */
    __forceinline bool svmLock() const
    {
        return this->bit.svmLock;
    }

    /**
     * @brief Checks if SVM is disabled.
     *
     * @return True if SVM is disabled, otherwise false
     */
    __forceinline bool svmeDisable() const
    {
        return this->bit.svmeDisable;
    }

    /**
     * @brief Reads the value of the MSR_VM_CR register.
     */
    __forceinline void ReadValue()
    {
        this->value = __readmsr( SVM_MSR_VM_CR );
    }

    /**
     * @brief Gets the value of the MSR_VM_CR register.
     *
     * @return The value of the MSR_VM_CR register
     */
    __forceinline auto GetValue() const
    {
        return this->value;
    }

    /**
     * @brief Writes the current value to the MSR_VM_CR register.
     */
    __forceinline void WriteValue() const
    {
        __writemsr( SVM_MSR_VM_CR, this->value );
    }

    /**
     * @brief Locks the SVM and writes the value to the MSR_VM_CR register.
     */
    __forceinline void LockAndWrite()
    {
        this->bit.svmeDisable = 0;
        this->bit.svmLock = 1;
        __writemsr( SVM_MSR_VM_CR, this->value );
    }
};

/**
 * @brief Represents the general-purpose registers of a guest.
 */
struct _GUEST_REGISTERS
{
    _GP_REGISTER r15; ///< General-purpose register R15
    _GP_REGISTER r14; ///< General-purpose register R14
    _GP_REGISTER r13; ///< General-purpose register R13
    _GP_REGISTER r12; ///< General-purpose register R12
    _GP_REGISTER r11; ///< General-purpose register R11
    _GP_REGISTER r10; ///< General-purpose register R10
    _GP_REGISTER r9; ///< General-purpose register R9
    _GP_REGISTER r8; ///< General-purpose register R8
    _GP_REGISTER rdi; ///< Destination index register
    _GP_REGISTER rsi; ///< Source index register
    _GP_REGISTER rbp; ///< Base pointer register
    _GP_REGISTER rsp; ///< Stack pointer register
    _GP_REGISTER rbx; ///< Base register
    _GP_REGISTER rdx; ///< Data register
    _GP_REGISTER rcx; ///< Counter register
    _GP_REGISTER rax; ///< Accumulator register
};

/**
 * @brief Represents the context of a guest during a VM exit.
 */
struct _EXIT_CONTEXT
{
    _GUEST_REGISTERS* guestRegisters; ///< Pointer to the guest's general-purpose registers
    bool exit; ///< Indicates if the VM exit was triggered
};



#endif // !CPU_HXX