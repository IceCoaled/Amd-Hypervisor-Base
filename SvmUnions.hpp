#ifndef SVMUNIONS_HPP
#define SVMUNIONS_HPP

#include <cstdint>

#pragma pack(push, 1)

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


union _GUEST_ASID_TLB_CONTROL
{
    uintmax_t value;
    struct 
    {
        uintmax_t guest_asid : 32;        
        uintmax_t tlb_control : 8;
        uintmax_t reserved0 : 24;
    };

#define TLB_FLUSH_ALL (0x00001)
#define TLB_FLUSH_GLOBAL_GUEST_TLB (0x00003)
#define TLB_FLUSH_ASID (0x00007)
};


union _VIRTUAL_INTR
{
    uintmax_t value;
    struct 
    {
        uintmax_t vTprValue : 4;
        uintmax_t vTprIgn : 4;
        uintmax_t vIrq : 1; 
        uintmax_t reserved0 : 7;  
        uintmax_t interruptPrio : 4; 
        uintmax_t ignVTpr : 1; 
        uintmax_t reserved1 : 3;
        uintmax_t masking : 1; 
        uintmax_t reserved2 : 1;
        uintmax_t nmiEnable : 1;
        uintmax_t reserved3 : 3;
        uintmax_t x2Avic : 1; 
        uintmax_t avicEnable : 1;
        uintmax_t intrVector : 8; 
        uintmax_t reserved4 : 24;
    };
};


union _NP_ENABLE
{
	uintmax_t value;
	struct
	{
		uintmax_t npEnable : 1;
		uintmax_t enableSEV : 1;
        uintmax_t enableEncryptedState : 1;
        uintmax_t guestExecuteTrap : 1;
        uintmax_t sssCheckEn : 1;
        uintmax_t transparentEncryption : 1;
        uintmax_t readOnlyNp : 1;
        uintmax_t enableInvlpgb : 1;
        uintmax_t reserved0 : 56;
	};
};


union _VMCB_CLEAN
{
    uint32_t value;
    struct
    {
        uint32_t intercepts : 1;
        uint32_t iomsrpm : 1;
        uint32_t asid : 1;
        uint32_t tpr : 1;
        uint32_t np : 1;
        uint32_t crX : 1;
        uint32_t drX : 1;
        uint32_t dt : 1;
        uint32_t seg : 1;
        uint32_t cr2 : 1;
        uint32_t lbr : 1;
        uint32_t avic : 1;
        uint32_t cet : 1;
        uint32_t reserved : 19;
    }bit;
};


union _EVENTINJ
{   
    uintmax_t value;
    struct
    {
        uintmax_t vector : 8;          // [0:7]
        uintmax_t type : 3;            // [8:10]
        uintmax_t errorCodeValid : 1;  // [11]
        uintmax_t reserved0 : 19;      // [12:30]
        uintmax_t valid : 1;           // [31]
        uintmax_t errorCode : 32;      // [32:63]
    }bit;
};


union _SEV_FEATURES
{
    uintmax_t value;
    struct
    {
        uintmax_t sevMask : 62;
        uintmax_t reserved : 1;
        uintmax_t Features : 1;
    };
};


union _GUEST_SEV_FEATURES
{
    uintmax_t value;
    struct
    {
        uintmax_t guestFeatures : 62;
        uintmax_t reserved : 2;
    };
};


union _EFER_REGISTER
{ 
private:
    uintmax_t value;
public:
    struct
    {
        uintmax_t sce : 1;
        uintmax_t reserved0 : 7;
        uintmax_t lme : 1;
        uintmax_t reserved1 : 1;
        uintmax_t lma : 1;
        uintmax_t noExecute : 1;
        uintmax_t svme : 1;
        uintmax_t lmsle : 1;
        uintmax_t ffxsr : 1;
        uintmax_t tce : 1;
        uintmax_t reserved2 : 1;
        uintmax_t mcommit : 1;
        uintmax_t intwb : 1;
        uintmax_t reserved3 : 1;
        uintmax_t uaie : 1;
        uintmax_t aibrse : 1;
        uintmax_t reserved4 : 42;
    }bit; 

    void SetSvmeBit()
	{
		value |= (1ULL << 12);
	}

    void ClearSvmeBit()
    {
        value &= ~(1ULL << 12);
    }

    void WriteValue() const
    {
        __writemsr(MSR_EFER, this->value);
    }

    bool cmpSvmeBit(__in const _EFER_REGISTER other) const
	{
		return this->bit.svme == other.bit.svme;
	}

    bool cmpNxeBit(__in const _EFER_REGISTER other) const
    {
        return this->bit.noExecute == other.bit.noExecute;
    }

    bool cmpLmeBit(__in const _EFER_REGISTER other) const
	{
		return this->bit.lme == other.bit.lme;
	}

    bool cmpLmaBit(__in const _EFER_REGISTER other) const
    {
        return this->bit.lma == other.bit.lma;
    }

    void ReadValue()
    {
        this->value = __readmsr(MSR_EFER);
    }

    __forceinline void operator=(__in const uintmax_t& val)
	{
		this->value = val;
	}

    __forceinline auto operator==(__in const uintmax_t& val) const
    {
        return this->value == val;
    }

    __forceinline auto operator!=(__in const uintmax_t& val) const
    {
        return this->value != val;
    }
};


union _CR0_REGISTER
{
    uintmax_t value;

    struct
    {
        uintmax_t pe : 1;
        uintmax_t mp : 1;
        uintmax_t em : 1;
        uintmax_t ts : 1;
        uintmax_t et : 1;
        uintmax_t ne : 1;
        uintmax_t reserved0 : 10;
        uintmax_t wp : 1;
        uintmax_t r : 1;
        uintmax_t reserved1 : 10;
        uintmax_t nw : 1;
        uintmax_t cd : 1;
        uintmax_t pg : 1;
    }bit;
};


union _CR3_REGISTER
{
    uintmax_t value; //if cr4.pcide is set, use pcideOn
    union
    {
        struct
        {
            uint64_t reserved0 : 3;
            uint64_t writeThrough : 1;
            uint64_t cacheDisabled : 1;
            uint64_t reserved1 : 7;
            uint64_t pml4 : 40;
            uint64_t reserved2 : 12;
        };
        struct
        {
            uint64_t pci : 12;	// pci: process-context identifier
            uint64_t pml4 : 40;
            uint64_t reserved3 : 12;
        }pcideOn;
    };
};


union _CR4_REGISTER
{
    uintmax_t value;
    struct
    {
        uintmax_t vme : 1;
        uintmax_t pvi : 1;
        uintmax_t tsd : 1;
        uintmax_t de : 1;
        uintmax_t pse : 1;
        uintmax_t pae : 1;
        uintmax_t mce : 1;
        uintmax_t pge : 1;
        uintmax_t pce : 1;
        uintmax_t osfxsr : 1;
        uintmax_t osxmmexcpt : 1;
        uintmax_t umip : 1;
        uintmax_t la57 : 1;
        uintmax_t reserved0 : 3;
        uintmax_t fsgsbase : 1;
        uintmax_t pcide : 1;
        uintmax_t osxsave : 1;
        uintmax_t reserved1 : 1;
        uintmax_t smep : 1;
        uintmax_t smap : 1;
        uintmax_t pke : 1;
        uintmax_t cet : 1;
        uintmax_t reserved2 : 8;
        uintmax_t reserved3 : 32;
    }bit;
};


union _SPEC_CNTRL
{
    uintmax_t value;
    struct
    {
        uintmax_t ibrs : 1;
        uintmax_t stibp : 1;
        uintmax_t ssbd : 1;
        uintmax_t reserved0 : 4;
        uintmax_t psfd : 1;
        uintmax_t reserved1 : 56;
    }bit;
};


union _RFLAGS_REGISTER
{
    uint64_t value;
    struct
    {
        uint64_t carryFlag : 1;
        uint64_t reserved0 : 1;
        uint64_t parityFlag : 1;
        uint64_t reserved1 : 1;
        uint64_t adjustFlag : 1;
        uint64_t reserved2 : 1;
        uint64_t zeroFlag : 1;
        uint64_t signFlag : 1;
        uint64_t trapFlag : 1;
        uint64_t interruptFlag : 1;
        uint64_t directionFlag : 1;
        uint64_t overflowFlag : 1;
        uint64_t ioPrivilegeLevel : 2;
        uint64_t nestedTask : 1;
        uint64_t reserved3 : 1;
        uint64_t resume : 1;
        uint64_t virtual8086 : 1;
        uint64_t alignmentCheck : 1;
        uint64_t virtualInterrupt : 1;
        uint64_t virtualInterruptPending : 1;
        uint64_t cpuid : 1;
        uint64_t reserved4 : 42;
    }bit;
};


#pragma pack(pop)


struct _INTERRUPT_REGISTER
{
    uint32_t apicOffset200;  //0:15 reserved, 16:31 apic offset 200
    uint32_t apicOffset210;
    uint32_t apicOffset220;
    uint32_t apicOffset230;
    uint32_t apicOffset240;
    uint32_t apicOffset250;
    uint32_t apicOffset260;
    uint32_t apicOffset270;
};


#endif // !SVMUNIONS_HPP

