#ifndef SVM_HPP
#define SVM_HPP

#include "Defines.hpp"
#include "Cpu.hpp"
#include "Memory.hpp"
#include "SvmUnions.hpp"
#include <ntddk.h>


/**
 * @brief Represents the control area of a Virtual Machine Control Block (VMCB).
 *
 * This structure is used to manage the control fields for a virtual machine.
 */
struct _VMCB_CONTROL_AREA
{
    _VECTOR_0 vector0;                    ///< Vector for control settings (0)
    _VECTOR_1 vector1;                    ///< Vector for control settings (1)
    _VECTOR_2 vector2;                    ///< Vector for control settings (2)
    _VECTOR_3 vector3;                    ///< Vector for control settings (3)
    _VECTOR_4 vector4;                    ///< Vector for control settings (4)
    _VECTOR_5 vector5;                    ///< Vector for control settings (5)
    uint8_t reserved0[ 0x03C - 0x018 ];     ///< Reserved space
    uint16_t pauseFilterThreshold;        ///< Pause filter threshold
    uint16_t pauseFilterCount;            ///< Pause filter count
    uintmax_t iopmBasePa;                 ///< Physical address of I/O permission bitmap
    uintmax_t msrpmBasePa;                ///< Physical address of MSR permission bitmap
    uintmax_t tscOffset;                  ///< TSC offset value
    _GUEST_ASID guestAsid;                ///< Guest address space identifier (ASID)
    _TLB_CONTROL TlbControl;              ///< TLB control settings
    _VIRTUAL_INTR vIntr;                  ///< Virtual interrupt settings
    uintmax_t interruptShadow;            ///< Interrupt shadow
    uintmax_t exitCode;                   ///< Exit code indicating reason for VM exit
    uintmax_t exitInfo1;                  ///< Additional information about the exit (1)
    uintmax_t exitInfo2;                  ///< Additional information about the exit (2)
    uintmax_t exitIntInfo;                ///< Exit interrupt information
    _NP_ENABLE npEnable;                  ///< Nested page enable settings
    uintmax_t avicApicBar;                ///< APIC base address register
    uintmax_t guestPaOfGhcb;              ///< Guest physical address of the GHCB
    uintmax_t eventInj;                   ///< Event injection information
    uintmax_t nCr3;                       ///< Nested page CR3 register
    uintmax_t lbrVirtualizationEnable;    ///< LBR virtualization enable
    _VMCB_CLEAN vmcbClean;                ///< VMCB clean field
    uintmax_t nRip;                       ///< Next RIP to execute after VM exit
    uint8_t numOfBytesFetched;            ///< Number of bytes fetched for guest instruction
    uint8_t guestInstructionBytes[ 15 ];    ///< Guest instruction bytes fetched
    uintmax_t avicApicBackingPagePointer; ///< Pointer to AVIC APIC backing page
    uintmax_t reserved1;                  ///< Reserved field
    uintmax_t avicLogicalTablePointer;    ///< Pointer to AVIC logical table
    uintmax_t avicPhysicalTablePointer;   ///< Pointer to AVIC physical table
    uintmax_t reserved2;                  ///< Reserved field
    uintmax_t VmcbSaveStatePointer;       ///< Pointer to VMCB save state
    uintmax_t vmgExitRax;                 ///< RAX value for VMG exit
    uint8_t vmgExitCpl;                   ///< Current privilege level for VMG exit
    uintmax_t busLockCount;               ///< Bus lock count
    uint8_t reserved3[ 0x134 - 0x128 ];     ///< Reserved space
    bool updateIrr;                       ///< Flag to indicate if IRR should be updated
    _SEV_FEATURES allowedSevFeatures;     ///< Allowed SEV features
    _GUEST_SEV_FEATURES guestSevFeatures; ///< Guest SEV features
    uintmax_t reserved4;                  ///< Reserved field
    _INTERRUPT_REGISTER irr;              ///< Interrupt request register (IRR)
    uint8_t reserved5[ 0x3E0 - 0x170 ];     ///< Reserved space
    uint8_t hostUsage[ 0x400 - 0x3E0 ];     ///< Host usage space
};


/**
 * @brief Represents the state save area of a Virtual Machine Control Block (VMCB).
 *
 * This structure is used to save the state of the virtual machine.
 */
struct _VMCB_STATE_SAVE_AREA
{
    _SEGMENT_REGISTER es;                ///< Segment register ES
    _SEGMENT_REGISTER cs;                ///< Segment register CS
    _SEGMENT_REGISTER ss;                ///< Segment register SS
    _SEGMENT_REGISTER ds;                ///< Segment register DS
    _SEGMENT_REGISTER fs;                ///< Segment register FS
    _SEGMENT_REGISTER gs;                ///< Segment register GS
    _SEGMENT_REGISTER gdtr;              ///< Global descriptor table register
    _SEGMENT_REGISTER ldtr;              ///< Local descriptor table register
    _SEGMENT_REGISTER idtr;              ///< Interrupt descriptor table register
    _SEGMENT_REGISTER tr;                ///< Task register
    uint8_t reserved0[ 0x0CB - 0x0A0 ];    ///< Reserved space
    uint8_t cpl;                         ///< Current privilege level
    uint32_t reserved1;                  ///< Reserved space
    _EFER_REGISTER efer;                 ///< Extended feature enable register
    uint8_t reserved2[ 0x148 - 0x0D8 ];    ///< Reserved space
    _CR4_REGISTER cr4;                   ///< Control register CR4
    _CR3_REGISTER cr3;                   ///< Control register CR3
    _CR0_REGISTER cr0;                   ///< Control register CR0
    uintmax_t dr7;                       ///< Debug register DR7
    uintmax_t dr6;                       ///< Debug register DR6
    _RFLAGS_REGISTER rflags;             ///< RFLAGS register
    uintmax_t rip;                       ///< Instruction pointer register
    uint8_t reserved3[ 0x1D8 - 0x180 ];    ///< Reserved space
    uintmax_t rsp;                       ///< Stack pointer register
    uintmax_t scet;                      ///< SCET register
    uintmax_t ssp;                       ///< Saved stack pointer
    uintmax_t isstAddr;                  ///< ISST address
    uintmax_t rax;                       ///< General-purpose register RAX
    uintmax_t star;                      ///< STAR register
    uintmax_t lStar;                     ///< LSTAR register
    uintmax_t cStar;                     ///< CSTAR register
    uintmax_t sfMask;                    ///< SF mask register
    uintmax_t kernelGsBase;              ///< Kernel GS base register
    uintmax_t sysenterCs;                ///< SYSENTER_CS register
    uintmax_t sysenterEsp;               ///< SYSENTER_ESP register
    uintmax_t sysenterEip;               ///< SYSENTER_EIP register
    uintmax_t cr2;                       ///< Control register CR2
    uint8_t reserved5[ 0x268 - 0x248 ];    ///< Reserved space
    uintmax_t gPat;                      ///< PAT register
    uintmax_t dbgCtl;                    ///< Debug control register
    uintmax_t brFrom;                    ///< Branch from address
    uintmax_t brTo;                      ///< Branch to address
    uintmax_t lastExcepFrom;             ///< Last exception from address
    uintmax_t lastExcepTo;               ///< Last exception to address
    uintmax_t dbgExtnCtl;                ///< Debug extension control
    uint8_t reserved6[ 0x2E0 - 0x298 ];    ///< Reserved space
    _SPEC_CNTRL specCntrl;               ///< Speculative control register
    uint8_t reserved7[ 0x670 - 0x2E8 ];    ///< Reserved space
    uintmax_t lbrStack[ 4 ];               ///< Last branch record stack
    uintmax_t lastBranchSelect;          ///< Last branch select
    uintmax_t ibsFetchCtl;               ///< IBS fetch control
    uintmax_t ibsFetchLinearAddr;        ///< IBS fetch linear address
    uintmax_t ibsOpCtl;                  ///< IBS operation control
    uintmax_t ibsOpRip;                  ///< IBS operation RIP
    uintmax_t ibsOpData0;                ///< IBS operation data 0
    uintmax_t ibsOpData1;                ///< IBS operation data 1
    uintmax_t ibsOpData2;                ///< IBS operation data 2
    uintmax_t ibsDcLinearAddr;           ///< IBS DC linear address
    uintmax_t BpIbstgtRip;               ///< BP IBS target RIP
    uintmax_t icIbsExtdCtl;              ///< IC IBS extended control
    uint8_t reserved8[ 0x50C ];            ///< Reserved space
};


/**
 * @brief Represents a Virtual Machine Control Block (VMCB).
 *
 * This structure contains both the control area and state save area for a virtual machine.
 */
struct _VMCB
{
    _VMCB_CONTROL_AREA controlArea;      ///< Control area of the VMCB
    _VMCB_STATE_SAVE_AREA stateSaveArea; ///< State save area of the VMCB
    static_assert( sizeof( controlArea ) == ( PAGE_SIZE / 4 ), "Vmcb State Save Area Out Of Alignment" );
};
static_assert( sizeof( _VMCB ) == PAGE_SIZE, "Vmcb State Save Area Out Of Alignment" );


/**
 * @brief Represents the CPU structure used by the hypervisor.
 *
 * This structure contains information about the host stack, guest VMCB, host VMCB, and more.
 */
struct _HV_CPU
{
    union
    {
        __declspec( align( PAGE_SIZE ) ) uint8_t hostStackLimit[ KERNEL_STACK_SIZE ]; ///< Host stack limit
        struct
        {
            uint8_t stackContents[ KERNEL_STACK_SIZE - ( sizeof( void* ) * 6 ) ]; ///< Contents of the stack
            uintmax_t guestVmcbPa;                 ///< Guest VMCB physical address
            uintmax_t hostVmcbPa;                  ///< Host VMCB physical address
            struct _HV_CPU* hvProcessor;           ///< Pointer to the hypervisor CPU structure
            _SHARED_PAGES* nestedPages;            ///< Pointer to nested pages structure
            _XCR0_REGISTER xSetBvMask;             ///< XCR0 register mask
            uintmax_t reserved0;                   ///< Reserved for safety check
        }layout;
    }hostStack;

    __declspec( align( PAGE_SIZE ) ) struct _VMCB guestVmcb; ///< Guest VMCB
    __declspec( align( PAGE_SIZE ) ) struct _VMCB hostVmcb;  ///< Host VMCB
    __declspec( align( PAGE_SIZE ) ) uint8_t hostStateArea[ PAGE_SIZE ]; ///< Host state area
};


/**
 * @brief Represents setup flags for the hypervisor.
 *
 * This structure is used to store flags that determine the operational state of the hypervisor.
 */
struct _SETUP_FLAGS
{
    uint32_t value; ///< Setup flags value
    union
    {
        uint32_t shadowStackOperational : 1; ///< Indicates if shadow stack is operational
        uint32_t pcideEnabled : 1;            ///< Indicates if PCIDE is enabled
        uint32_t level5Paging : 1;            ///< Indicates if level 5 paging is enabled
        uint32_t x2ApicEnabled : 1;           ///< Indicates if x2APIC is enabled
    }bit;
#define SHADOW_STACK_OPERATIONAL (0x000000001)
#define PCIDE_ENABLED (0x000000002)
#define LEVEL5_PAGING (0x000000004)
#define X2APIC_ENABLED (0x00000008)
};


/**
 * @brief Represents the context of a guest virtual machine.
 *
 * This structure is used to store the context of a guest, including control registers and segment selectors.
 */
struct _GUEST_CONTEXT
{
    _DESCRIPTOR_REGISTER gdtr; ///< Global descriptor table register
    _DESCRIPTOR_REGISTER idtr; ///< Interrupt descriptor table register

    uint16_t cs; ///< Code segment selector
    uint16_t ds; ///< Data segment selector
    uint16_t es; ///< Extra segment selector
    uint16_t ss; ///< Stack segment selector

    uintmax_t cr0;    ///< Control register CR0
    uintmax_t cr3;    ///< Control register CR3
    uintmax_t cr4;    ///< Control register CR4
    uintmax_t cr2;    ///< Control register CR2
    uintmax_t rflags; ///< Flags register, used to store the current state of the processor
    uintmax_t rip;    ///< Instruction pointer register, stores the address of the next instruction to execute
    uintmax_t rsp;    ///< Stack pointer register, points to the top of the stack
    uintmax_t efer;   ///< Extended feature enable register
    uintmax_t gPat;   ///< Page attribute table, used to configure the memory cache control
};

#endif // !SVM_HXX