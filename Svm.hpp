#ifndef SVM_HPP
#define SVM_HPP

#include "Defines.hpp"
#include "Cpu.hpp"
#include "Memory.hpp"
#include "SvmUnions.hpp"
#include <ntddk.h>



#define LAYOUT_SIZE 0x5E48
#define HV_CPU_SIZE 0xA000

struct _VMCB_CONTROL_AREA
{
    _VECTOR_0 vector0;
    _VECTOR_1 vector1;
    _VECTOR_2 vector2;
    _VECTOR_3 vector3;
    _VECTOR_4 vector4;
    _VECTOR_5 vector5;
    uint8_t reserved0[0x03C - 0x018];     
    uint16_t pauseFilterThreshold;        
    uint16_t pauseFilterCount;            
    uintmax_t iopmBasePa;                 
    uintmax_t msrpmBasePa;                
    uintmax_t tscOffset;                  
    _GUEST_ASID_TLB_CONTROL guestAsidTlbControl;
    _VIRTUAL_INTR vIntr;
    uintmax_t interruptShadow;            
    uintmax_t exitCode;                   
    uintmax_t exitInfo1;
    uintmax_t exitInfo2;                  
    uintmax_t exitIntInfo;                
    _NP_ENABLE npEnable;
    uintmax_t avicApicBar;//51:0 apic_bar, 63:52 reserved                
    uintmax_t guestPaOfGhcb;              
    uintmax_t eventInj;                   
    uintmax_t nCr3;                       
    uintmax_t lbrVirtualizationEnable;    
    _VMCB_CLEAN vmcbClean;                  
    uintmax_t nRip;                       
    uint8_t numOfBytesFetched;            
    uint8_t guestInstructionBytes[15];    
    uintmax_t avicApicBackingPagePointer; 
    uintmax_t reserved1;                  
    uintmax_t avicLogicalTablePointer;    
    uintmax_t avicPhysicalTablePointer;   
    uintmax_t reserved2;                  
    uintmax_t VmcbSaveStatePointer;       
    uintmax_t vmgExitRax;
    uint8_t vmgExitCpl;
    uintmax_t busLockCount;  
    uint8_t reserved3[0x134 - 0x128];
    bool updateIrr;
    _SEV_FEATURES allowedSevFeatures;
    _GUEST_SEV_FEATURES guestSevFeatures;
    uintmax_t reserved4;
    _INTERRUPT_REGISTER irr;
    uint8_t reserved5[0x3E0 - 0x170];
    uint8_t hostUsage[0x400 - 0x3E0];
};


struct _VMCB_STATE_SAVE_AREA
{
    _SEGMENT_REGISTER es;
    _SEGMENT_REGISTER cs;
    _SEGMENT_REGISTER ss;
    _SEGMENT_REGISTER ds;
    _SEGMENT_REGISTER fs;
    _SEGMENT_REGISTER gs;
    _SEGMENT_REGISTER gdtr;
    _SEGMENT_REGISTER ldtr;
    _SEGMENT_REGISTER idtr;
    _SEGMENT_REGISTER tr;
    uint8_t reserved0[0x0CB - 0x0A0];     
    uint8_t cpl;                          
    uint32_t reserved1;                   
    _EFER_REGISTER efer;
    uint8_t reserved2[0x148 - 0x0D8];     
    _CR4_REGISTER cr4;
    _CR3_REGISTER cr3;
    _CR0_REGISTER cr0;
    uintmax_t dr7;                        
    uintmax_t dr6;                        
    _RFLAGS_REGISTER rflags;                     
    uintmax_t rip;                        
    uint8_t reserved3[0x1D8 - 0x180];     
    uintmax_t rsp;                        
    uintmax_t scet;
    uintmax_t ssp;
    uintmax_t isstAddr;
    uintmax_t rax;                        
    uintmax_t star;                       
    uintmax_t lStar;                      
    uintmax_t cStar;                      
    uintmax_t sfMask;                     
    uintmax_t kernelGsBase;               
    uintmax_t sysenterCs;                 
    uintmax_t sysenterEsp;                
    uintmax_t sysenterEip;                
    uintmax_t cr2;                        
    uint8_t reserved5[0x268 - 0x248];     
    uintmax_t gPat;                       
    uintmax_t dbgCtl;                     
    uintmax_t brFrom;                     
    uintmax_t brTo;                       
    uintmax_t lastExcepFrom;              
    uintmax_t lastExcepTo; 
    uintmax_t dbgExtnCtl;
    uint8_t reserved6[0x2E0 - 0x298];
    _SPEC_CNTRL specCntrl;
    uint8_t reserved7[0x670 - 0x2E8];
    uintmax_t lbrStack[4];
    uintmax_t lastBranchSelect;
    uintmax_t ibsFetchCtl;
    uintmax_t ibsFetchLinearAddr;
    uintmax_t ibsOpCtl;
    uintmax_t ibsOpRip;
    uintmax_t ibsOpData0;
    uintmax_t ibsOpData1;
    uintmax_t ibsOpData2;
    uintmax_t ibsDcLinearAddr;
    uintmax_t BpIbstgtRip;
    uintmax_t icIbsExtdCtl;
    uint8_t reserved8[0x50C];
};



struct _VMCB
{
    _VMCB_CONTROL_AREA controlArea;
    _VMCB_STATE_SAVE_AREA stateSaveArea;
    static_assert(sizeof(controlArea) == (PAGE_SIZE / 4), "Vmcb State Save Area Out Of Alignment");
};
static_assert(sizeof(_VMCB) == PAGE_SIZE, "Vmcb State Save Area Out Of Alignment");




struct _HV_CPU
{ 
    union
    {
        __declspec(align(PAGE_SIZE)) uint8_t hostStackLimit[KERNEL_STACK_SIZE];
        struct
        {
            uint8_t stackContents[LAYOUT_SIZE];
            _GUEST_REGISTERS guestRegisters;
            uintmax_t guestVmcbPa;
            uintmax_t hostVmcbPa;
            _HV_CPU* hvProcessor;
            _SHARED_PAGES* nestedPages;
            _SHARED_PAGES* hostPages;
            _EFER_REGISTER eferBitMask;       
            _XCR0_REGISTER xSetBvMask;
        }layout;
    }hostStack;

    __declspec(align(PAGE_SIZE)) _VMCB guestVmcb;
    __declspec(align(PAGE_SIZE)) _VMCB hostVmcb;
    __declspec(align(PAGE_SIZE)) uint8_t hostStateArea[PAGE_SIZE];
};
static_assert(sizeof(_HV_CPU) == HV_CPU_SIZE, "HV_CPU size mismatch");




struct _SETUP_FLAGS
{
    uint32_t value;
    union
    {
        uint32_t shadowStackEnabled : 1;
        uint32_t shadowStackOperational : 1;
        uint32_t pcideEnabled : 1;
        uint32_t level5Paging : 1;
        uint32_t x2ApicEnabled : 1;
        uint32_t apicEnabled : 1;
    }bit;
#define SHADOW_STACK_ENABLED (0x000000001)
#define SHADOW_STACK_OPERATIONAL (0x000000002)
#define PCIDE_ENABLED (0x000000004)
#define LEVEL5_PAGING (0x000000008)
#define X2APIC_ENABLED (0x000000010)
#define APIC_ENABLED (0x000000020)
};

#endif // !SVM_HXX