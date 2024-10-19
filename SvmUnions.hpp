#ifndef SVMUNIONS_HPP
#define SVMUNIONS_HPP

#include <cstdint>

#pragma pack(push, 1)

/**
 * @brief Represents a set of control register read/write access flags.
 *
 * This union is used to store flags indicating which control registers are being accessed.
 */
union _VECTOR_0
{
    uint32_t value;
    struct
    {
        uint32_t cr0_read : 1;
        uint32_t cr1_read : 1;
        uint32_t cr2_read : 1;
        uint32_t cr3_read : 1;
        uint32_t cr4_read : 1;
        uint32_t cr5_read : 1;
        uint32_t cr6_read : 1;
        uint32_t cr7_read : 1;
        uint32_t cr8_read : 1;
        uint32_t cr9_read : 1;
        uint32_t cr10_read : 1;
        uint32_t cr11_read : 1;
        uint32_t cr12_read : 1;
        uint32_t cr13_read : 1;
        uint32_t cr14_read : 1;
        uint32_t cr15_read : 1;
        uint32_t cr0_write : 1;
        uint32_t cr1_write : 1;
        uint32_t cr2_write : 1;
        uint32_t cr3_write : 1;
        uint32_t cr4_write : 1;
        uint32_t cr5_write : 1;
        uint32_t cr6_write : 1;
        uint32_t cr7_write : 1;
        uint32_t cr8_write : 1;
        uint32_t cr9_write : 1;
        uint32_t cr10_write : 1;
        uint32_t cr11_write : 1;
        uint32_t cr12_write : 1;
        uint32_t cr13_write : 1;
        uint32_t cr14_write : 1;
        uint32_t cr15_write : 1;
    }bit;
};

/**
 * @brief Represents a set of debug register read/write access flags.
 *
 * This union is used to store flags indicating which debug registers are being accessed.
 */
union _VECTOR_1
{
    uint32_t value;
    struct
    {
        uint32_t dr0_read : 1;
        uint32_t dr1_read : 1;
        uint32_t dr2_read : 1;
        uint32_t dr3_read : 1;
        uint32_t dr4_read : 1;
        uint32_t dr5_read : 1;
        uint32_t dr6_read : 1;
        uint32_t dr7_read : 1;
        uint32_t dr8_read : 1;
        uint32_t dr9_read : 1;
        uint32_t dr10_read : 1;
        uint32_t dr11_read : 1;
        uint32_t dr12_read : 1;
        uint32_t dr13_read : 1;
        uint32_t dr14_read : 1;
        uint32_t dr15_read : 1;
        uint32_t dr0_write : 1;
        uint32_t dr1_write : 1;
        uint32_t dr2_write : 1;
        uint32_t dr3_write : 1;
        uint32_t dr4_write : 1;
        uint32_t dr5_write : 1;
        uint32_t dr6_write : 1;
        uint32_t dr7_write : 1;
        uint32_t dr8_write : 1;
        uint32_t dr9_write : 1;
        uint32_t dr10_write : 1;
        uint32_t dr11_write : 1;
        uint32_t dr12_write : 1;
        uint32_t dr13_write : 1;
        uint32_t dr14_write : 1;
        uint32_t dr15_write : 1;
    }bit;
};

/**
 * @brief Represents exception vector information.
 *
 * This union is used to store flags indicating exceptions, such as divide errors, debug breakpoints, and page faults.
 */
union _VECTOR_2
{
    uint32_t value;
    struct
    {
        uint32_t divideError : 1;
        uint32_t debugBreakPoint : 1;
        uint32_t nonMaskableInterrupt : 1;
        uint32_t breakPoint : 1;
        uint32_t overFlow : 1;
        uint32_t boundRangeExceeded : 1;
        uint32_t undefinedOpCode : 1;
        uint32_t noMathCoprocessor : 1;
        uint32_t doubleFault : 1;
        uint32_t reserved0 : 1;
        uint32_t invalidTaskSegmentSelector : 1;
        uint32_t segmentNotPresent : 1;
        uint32_t stackSegmentFault : 1;
        uint32_t generalProtection : 1;
        uint32_t pageFault : 1;
        uint32_t reserved1 : 1;
        uint32_t mathFault : 1;
        uint32_t alignmentCheck : 1;
        uint32_t machineCheck : 1;
        uint32_t sseError : 1;
        uint32_t virtualException : 1;
        uint32_t reserved2 : 1;
        uint32_t reserved3 : 1;
        uint32_t reserved4 : 1;
        uint32_t reserved5 : 1;
        uint32_t reserved6 : 1;
        uint32_t reserved7 : 1;
        uint32_t reserved8 : 1;
        uint32_t reserved9 : 1;
        uint32_t reserved10 : 1;
        uint32_t reserved11 : 1;
        uint32_t reserved12 : 1;
    }bit;
};

/**
 * @brief Represents virtualization event vectors.
 *
 * This union is used to store flags indicating different virtualization events, such as interrupts, NMI, and SMI.
 */
union _VECTOR_3
{
    uint32_t value;
    struct
    {
        uint32_t intr : 1;
        uint32_t nmi : 1;
        uint32_t smi : 1;
        uint32_t init : 1;
        uint32_t vinir : 1;
        uint32_t cr0 : 1;
        uint32_t readIdtr : 1;
        uint32_t readGdtr : 1;
        uint32_t readLdtr : 1;
        uint32_t readTr : 1;
        uint32_t writeIdtr : 1;
        uint32_t writeGdtr : 1;
        uint32_t writeLdtr : 1;
        uint32_t writeTr : 1;
        uint32_t rdtsc : 1;
        uint32_t rdpmc : 1;
        uint32_t pushf : 1;
        uint32_t popf : 1;
        uint32_t cpuid : 1;
        uint32_t rsm : 1;
        uint32_t iret : 1;
        uint32_t intN : 1;
        uint32_t invd : 1;
        uint32_t pause : 1;
        uint32_t hlt : 1;
        uint32_t invlpg : 1;
        uint32_t invlpga : 1;
        uint32_t ioioProto : 1;
        uint32_t msrProto : 1;
        uint32_t taskSwitch : 1;
        uint32_t ferrFreeze : 1;
        uint32_t shutdowns : 1;
    }bit;
};

/**
 * @brief Represents control fields for various virtualization actions.
 *
 * This union is used to store flags indicating actions like vmrun, vmsave, and clgi.
 */
union _VECTOR_4
{
    uint32_t vector3Value;
    struct
    {
        uint32_t vmRun : 1;
        uint32_t vmCall : 1;
        uint32_t vmLoad : 1;
        uint32_t vmSave : 1;
        uint32_t stgi : 1;
        uint32_t clgi : 1;
        uint32_t skinit : 1;
        uint32_t rdtscp : 1;
        uint32_t icebp : 1;
        uint32_t wbinvd : 1;
        uint32_t monitor : 1;
        uint32_t mwait : 1;
        uint32_t mwaitHardware : 1;
        uint32_t xsetbv : 1;
        uint32_t rdpru : 1;
        uint32_t eferAfterGuest : 1;
        uint32_t cr0To15AfterGuest : 16;
    }bit;
};

/**
 * @brief Represents control fields for specific virtualization instructions and features.
 *
 * This union is used to store flags indicating operations like INVLPGB and bus locking.
 */
union _VECTOR_5
{
    uint32_t vector4Value;
    struct
    {
        uint32_t invlpgb : 1;
        uint32_t illegalInvlpgb : 1;
        uint32_t invpicd : 1;
        uint32_t mcommit : 1;
        uint32_t tlbSync : 1;
        uint32_t busLock : 1;
        uint32_t hltNonVirtual : 1;
        uint32_t reserved : 25;
    }bit;
};

/**
 * @brief Represents the guest address space identifier (ASID).
 *
 * This union is used to store the ASID value for a guest.
 */
union _GUEST_ASID
{
    uint32_t value;
    struct
    {
        uint32_t guest_asid : 32;
    }bit;

#define TLB_FLUSH_ALL (0x00001)
#define TLB_FLUSH_GLOBAL_GUEST_TLB (0x00003)
#define TLB_FLUSH_ASID (0x00007)
};

/**
 * @brief Represents TLB control settings.
 *
 * This union is used to manage TLB control options.
 */

union _TLB_CONTROL
{
    uint32_t value;
    struct
    {
        uint32_t tlbControl : 8; ///< TLB control bits.
        uint32_t reserved0 : 24; ///< Reserved bits.
    }bit;
};

/**
 * @brief Represents a virtual interrupt control structure.
 *
 * This union stores the configuration for virtual interrupts, including priority and masking.
 */
union _VIRTUAL_INTR
{
    uintmax_t value;
    struct
    {
        uintmax_t vTprValue : 4; ///< Virtual Task Priority Register value.
        uintmax_t vTprIgn : 4; ///< Ignored bits for vTpr.
        uintmax_t vIrq : 1; ///< Virtual IRQ.
        uintmax_t reserved0 : 7; ///< Reserved bits.
        uintmax_t interruptPrio : 4; ///< Interrupt priority.
        uintmax_t ignVTpr : 1; ///< Ignore vTpr.
        uintmax_t reserved1 : 3; ///< Reserved bits.
        uintmax_t masking : 1; ///< Masking bit.
        uintmax_t reserved2 : 1; ///< Reserved bit.
        uintmax_t nmiEnable : 1; ///< NMI enable.
        uintmax_t reserved3 : 3; ///< Reserved bits.
        uintmax_t x2Avic : 1; ///< x2APIC virtualization enable.
        uintmax_t avicEnable : 1; ///< AVIC enable.
        uintmax_t intrVector : 8; ///< Interrupt vector.
        uintmax_t reserved4 : 24; ///< Reserved bits.
    };
};

/**
 * @brief Represents nested paging enable and other SEV-related settings.
 *
 * This union stores flags for enabling nested paging, SEV, and other security-related features.
 */
union _NP_ENABLE
{
    uintmax_t value;
    struct
    {
        uintmax_t npEnable : 1; ///< Nested paging enable.
        uintmax_t enableSEV : 1; ///< Secure Encrypted Virtualization enable.
        uintmax_t enableEncryptedState : 1; ///< Encrypted state enable.
        uintmax_t guestExecuteTrap : 1; ///< Guest execute trap enable.
        uintmax_t sssCheckEn : 1; ///< SSS check enable.
        uintmax_t transparentEncryption : 1; ///< Transparent encryption enable.
        uintmax_t readOnlyNp : 1; ///< Read-only nested paging.
        uintmax_t enableInvlpgb : 1; ///< INVLPGB enable.
        uintmax_t reserved0 : 56; ///< Reserved bits.
    };
};

/**
 * @brief Represents the VMCB clean fields to optimize context switching.
 *
 * This union stores flags to indicate which parts of the VMCB need to be updated during a VM exit.
 */
union _VMCB_CLEAN
{
    uint32_t value;
    struct
    {
        uint32_t intercepts : 1; ///< Intercepts clean.
        uint32_t iomsrpm : 1; ///< IOMSRPM clean.
        uint32_t asid : 1; ///< ASID clean.
        uint32_t tpr : 1; ///< TPR clean.
        uint32_t np : 1; ///< Nested paging clean.
        uint32_t crX : 1; ///< Control registers clean.
        uint32_t drX : 1; ///< Debug registers clean.
        uint32_t dt : 1; ///< Descriptor tables clean.
        uint32_t seg : 1; ///< Segment registers clean.
        uint32_t cr2 : 1; ///< CR2 register clean.
        uint32_t lbr : 1; ///< Last branch record clean.
        uint32_t avic : 1; ///< AVIC clean.
        uint32_t cet : 1; ///< CET clean.
        uint32_t reserved : 19; ///< Reserved bits.
    }bit;
};

/**
 * @brief Represents an event injection structure for virtualization.
 *
 * This union stores information for injecting an event, such as an interrupt or exception, into a guest.
 */
union _EVENTINJ
{
    uintmax_t value;
    struct
    {
        uintmax_t vector : 8; ///< Event vector (interrupt/exception).
        uintmax_t type : 3; ///< Type of event.
        uintmax_t errorCodeValid : 1; ///< Error code validity flag.
        uintmax_t reserved0 : 19; ///< Reserved bits.
        uintmax_t valid : 1; ///< Validity flag for the event.
        uintmax_t errorCode : 32; ///< Error code for the event.
    }bit;
};

/**
 * @brief Represents the SEV features supported by the guest.
 *
 * This union stores flags indicating supported SEV features for the virtual machine.
 */
union _SEV_FEATURES
{
    uintmax_t value;
    struct
    {
        uintmax_t sevMask : 62; ///< SEV feature mask.
        uintmax_t reserved : 1; ///< Reserved bit.
        uintmax_t Features : 1; ///< Indicates enabled SEV features.
    };
};

/**
 * @brief Represents SEV features specific to a guest virtual machine.
 *
 * This union stores flags indicating SEV features that are enabled for the guest.
 */
union _GUEST_SEV_FEATURES
{
    uintmax_t value;
    struct
    {
        uintmax_t guestFeatures : 62; ///< Guest SEV feature flags.
        uintmax_t reserved : 2; ///< Reserved bits.
    };
};

/**
 * @brief Represents the Extended Feature Enable Register (EFER).
 *
 * This union stores various EFER flags, including SVME and LMA, with methods to manipulate those flags.
 */
union _EFER_REGISTER
{
private:
    uintmax_t value;
public:
    struct
    {
        uintmax_t sce : 1; ///< System Call Extensions enable.
        uintmax_t reserved0 : 7; ///< Reserved bits.
        uintmax_t lme : 1; ///< Long Mode Enable.
        uintmax_t reserved1 : 1; ///< Reserved bit.
        uintmax_t lma : 1; ///< Long Mode Active.
        uintmax_t noExecute : 1; ///< No Execute enable.
        uintmax_t svme : 1; ///< Secure Virtual Machine enable.
        uintmax_t lmsle : 1; ///< Long Mode Segment Limit Enable.
        uintmax_t ffxsr : 1; ///< Fast FXSAVE/FXRSTOR enable.
        uintmax_t tce : 1; ///< Translation Cache Extension.
        uintmax_t reserved2 : 1; ///< Reserved bit.
        uintmax_t mcommit : 1; ///< MCOMMIT instruction enable.
        uintmax_t intwb : 1; ///< Interrupt window bitmap enable.
        uintmax_t reserved3 : 1; ///< Reserved bit.
        uintmax_t uaie : 1; ///< User Address Injection Enable.
        uintmax_t aibrse : 1; ///< AIBRSE enable.
        uintmax_t reserved4 : 42; ///< Reserved bits.
    }bit;

    /**
     * @brief Sets the SVME bit in the EFER register.
     */
    void SetSvmeBit()
    {
        value |= ( 1ULL << 12 );
    }

    /**
     * @brief Clears the SVME bit in the EFER register.
     */
    void ClearSvmeBit()
    {
        value &= ~( 1ULL << 12 );
    }

    /**
     * @brief Writes the value of the EFER register to the MSR.
     */
    void WriteValue() const
    {
        __writemsr( MSR_EFER, this->value );
    }

    /**
     * @brief Compares the SVME bit of this register with another EFER register.
     *
     * @param other Another EFER register to compare with.
     * @return True if the SVME bit matches, otherwise false.
     */
    bool cmpSvmeBit( __in const _EFER_REGISTER other ) const
    {
        return this->bit.svme == other.bit.svme;
    }

    /**
     * @brief Compares the NXE bit of this register with another EFER register.
     *
     * @param other Another EFER register to compare with.
     * @return True if the NXE bit matches, otherwise false.
     */
    bool cmpNxeBit( __in const _EFER_REGISTER other ) const
    {
        return this->bit.noExecute == other.bit.noExecute;
    }

    /**
     * @brief Compares the LME bit of this register with another EFER register.
     *
     * @param other Another EFER register to compare with.
     * @return True if the LME bit matches, otherwise false.
     */
    bool cmpLmeBit( __in const _EFER_REGISTER other ) const
    {
        return this->bit.lme == other.bit.lme;
    }

    /**
     * @brief Compares the LMA bit of this register with another EFER register.
     *
     * @param other Another EFER register to compare with.
     * @return True if the LMA bit matches, otherwise false.
     */
    bool cmpLmaBit( __in const _EFER_REGISTER other ) const
    {
        return this->bit.lma == other.bit.lma;
    }

    /**
     * @brief Reads the value of the EFER register from the MSR.
     */
    void ReadValue()
    {
        this->value = __readmsr( MSR_EFER );
    }

    /**
     * @brief Overloaded assignment operator for setting the value of the EFER register.
     *
     * @param val Value to assign to the register.
     */
    __forceinline void operator=( __in const uintmax_t& val )
    {
        this->value = val;
    }

    /**
     * @brief Overloaded equality operator for comparing the value of the EFER register.
     *
     * @param val Value to compare against.
     * @return True if the values are equal, otherwise false.
     */
    __forceinline auto operator==( __in const uintmax_t& val ) const
    {
        return this->value == val;
    }

    /**
     * @brief Overloaded inequality operator for comparing the value of the EFER register.
     *
     * @param val Value to compare against.
     * @return True if the values are not equal, otherwise false.
     */
    __forceinline auto operator!=( __in const uintmax_t& val ) const
    {
        return this->value != val;
    }
};



/**
 * @brief Represents the Extended Control Register (XCR0).
 *
 * This union stores various fields in XCR0, including the state of the x87 FPU, SSE, and AVX registers.
 */
union _CR0_REGISTER
{
    uintmax_t value;

    struct
    {
        uintmax_t pe : 1; ///< Protection Enable.
        uintmax_t mp : 1; ///< Monitor Coprocessor.
        uintmax_t em : 1; ///< Emulation.
        uintmax_t ts : 1; ///< Task Switched.
        uintmax_t et : 1; ///< Extension Type.
        uintmax_t ne : 1; ///< Numeric Error.
        uintmax_t reserved0 : 10; ///< Reserved bits.
        uintmax_t wp : 1; ///< Write Protect.
        uintmax_t r : 1; ///< Reserved bit.
        uintmax_t reserved1 : 10; ///< Reserved bits.
        uintmax_t nw : 1; ///< Not Write-through.
        uintmax_t cd : 1; ///< Cache Disable.
        uintmax_t pg : 1; ///< Paging.
    }bit;
};

/**
 * @brief Represents the Control Register 3 (CR3).
 *
 * This union stores various fields in CR3, including process-context identifier and page-level fields.
 */
union _CR3_REGISTER
{
    uintmax_t value; ///< Value of the CR3 register.
    union
    {
        struct
        {
            uintmax_t reserved0 : 3; ///< Reserved bits.
            uintmax_t writeThrough : 1; ///< Write-through caching.
            uintmax_t cacheDisabled : 1; ///< Cache disable flag.
            uintmax_t reserved1 : 7; ///< Reserved bits.
            uintmax_t pml4 : 40; ///< Page-map level-4 base address.
            uintmax_t reserved2 : 12; ///< Reserved bits.
        };
        struct
        {
            uintmax_t pci : 12; ///< Process-context identifier.
            uintmax_t pml4 : 40; ///< Page-map level-4 base address.
            uintmax_t reserved3 : 12; ///< Reserved bits.
        }pcideOn; ///< Fields when PCIDE is enabled.
    };
};

/**
 * @brief Represents the Control Register 4 (CR4).
 *
 * This union stores various fields and features enabled in CR4, including virtual memory and paging features.
 */
union _CR4_REGISTER
{
    uintmax_t value; ///< Value of the CR4 register.
    struct
    {
        uintmax_t vme : 1; ///< Virtual-8086 Mode Extensions.
        uintmax_t pvi : 1; ///< Protected-mode Virtual Interrupts.
        uintmax_t tsd : 1; ///< Time Stamp Disable.
        uintmax_t de : 1; ///< Debugging Extensions.
        uintmax_t pse : 1; ///< Page Size Extension.
        uintmax_t pae : 1; ///< Physical Address Extension.
        uintmax_t mce : 1; ///< Machine-Check Enable.
        uintmax_t pge : 1; ///< Page Global Enable.
        uintmax_t pce : 1; ///< Performance-Monitoring Counter Enable.
        uintmax_t osfxsr : 1; ///< Operating system FXSAVE/FXRSTOR support.
        uintmax_t osxmmexcpt : 1; ///< Operating system unmasked exception support.
        uintmax_t umip : 1; ///< User-Mode Instruction Prevention.
        uintmax_t la57 : 1; ///< 5-level paging enable.
        uintmax_t reserved0 : 3; ///< Reserved bits.
        uintmax_t fsgsbase : 1; ///< Enables RDFSBASE, WRFSBASE, RDGSBASE, WRGSBASE instructions.
        uintmax_t pcide : 1; ///< PCID enable.
        uintmax_t osxsave : 1; ///< XSAVE and processor extended states enable.
        uintmax_t reserved1 : 1; ///< Reserved bit.
        uintmax_t smep : 1; ///< Supervisor Mode Execution Protection.
        uintmax_t smap : 1; ///< Supervisor Mode Access Prevention.
        uintmax_t pke : 1; ///< Protection Key Enable.
        uintmax_t cet : 1; ///< Control-flow Enforcement Technology.
        uintmax_t reserved2 : 8; ///< Reserved bits.
        uintmax_t reserved3 : 32; ///< Reserved bits.
    }bit;
};

/**
 * @brief Represents the Speculation Control Register.
 *
 * This union is used to manage CPU speculation features such as IBRS and STIBP.
 */
union _SPEC_CNTRL
{
    uintmax_t value; ///< Value of the Speculation Control Register.
    struct
    {
        uintmax_t ibrs : 1; ///< Indirect Branch Restricted Speculation.
        uintmax_t stibp : 1; ///< Single Thread Indirect Branch Predictors.
        uintmax_t ssbd : 1; ///< Speculative Store Bypass Disable.
        uintmax_t reserved0 : 4; ///< Reserved bits.
        uintmax_t psfd : 1; ///< Predictive Store Forwarding Disable.
        uintmax_t reserved1 : 56; ///< Reserved bits.
    }bit;
};

/**
 * @brief Represents the RFLAGS Register.
 *
 * This union stores various flags that represent the state of the processor, such as carry flag, parity flag, and interrupt flag.
 */
union _RFLAGS_REGISTER
{
    uintmax_t value; ///< Value of the RFLAGS register.
    struct
    {
        uintmax_t carryFlag : 1; ///< Carry flag.
        uintmax_t reserved0 : 1; ///< Reserved bit.
        uintmax_t parityFlag : 1; ///< Parity flag.
        uintmax_t reserved1 : 1; ///< Reserved bit.
        uintmax_t adjustFlag : 1; ///< Adjust flag.
        uintmax_t reserved2 : 1; ///< Reserved bit.
        uintmax_t zeroFlag : 1; ///< Zero flag.
        uintmax_t signFlag : 1; ///< Sign flag.
        uintmax_t trapFlag : 1; ///< Trap flag.
        uintmax_t interruptFlag : 1; ///< Interrupt enable flag.
        uintmax_t directionFlag : 1; ///< Direction flag.
        uintmax_t overflowFlag : 1; ///< Overflow flag.
        uintmax_t ioPrivilegeLevel : 2; ///< I/O privilege level.
        uintmax_t nestedTask : 1; ///< Nested task flag.
        uintmax_t reserved3 : 1; ///< Reserved bit.
        uintmax_t resume : 1; ///< Resume flag.
        uintmax_t virtual8086 : 1; ///< Virtual-8086 mode flag.
        uintmax_t alignmentCheck : 1; ///< Alignment check flag.
        uintmax_t virtualInterrupt : 1; ///< Virtual interrupt flag.
        uintmax_t virtualInterruptPending : 1; ///< Virtual interrupt pending flag.
        uintmax_t cpuid : 1; ///< CPUID availability flag.
        uintmax_t reserved4 : 42; ///< Reserved bits.
    }bit;
};


#pragma pack(pop)


/**
 * @brief Represents the Interrupt Register structure.
 *
 * This struct contains the offsets for APIC (Advanced Programmable Interrupt Controller) registers.
 */
struct _INTERRUPT_REGISTER
{
    uint32_t apicOffset200;  ///< Offset for APIC register 200h.
    uint32_t apicOffset210;  ///< Offset for APIC register 210h.
    uint32_t apicOffset220;  ///< Offset for APIC register 220h.
    uint32_t apicOffset230;  ///< Offset for APIC register 230h.
    uint32_t apicOffset240;  ///< Offset for APIC register 240h.
    uint32_t apicOffset250;  ///< Offset for APIC register 250h.
    uint32_t apicOffset260;  ///< Offset for APIC register 260h.
    uint32_t apicOffset270;  ///< Offset for APIC register 270h.
};


#endif // !SVMUNIONS_HPP

