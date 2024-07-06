#include <ntifs.h>
#include <intrin.h>

#include "Hypervisor.hpp"
#include "CustomFunctions.hpp"


#pragma intrinsic(__segmentlimit)
#pragma intrinsic(__cpuid)
#pragma intrinsic(__readcr0)
#pragma intrinsic(__readcr2)
#pragma intrinsic(__readcr3)
#pragma intrinsic(__readcr4)
#pragma intrinsic(__readcr8)
#pragma intrinsic(__readmsr)
#pragma intrinsic( _xgetbv)
#pragma intrinsic(__sidt)
#pragma intrinsic(__writemsr)
#pragma intrinsic(__svm_vmsave)
#pragma intrinsic(__svm_vmload)
#pragma intrinsic(__getcallerseflags)



    void Hypervisor::BuildOutVmcb(__inout _HV_CPU* currentProcessor)
    {
        
        _DESCRIPTOR_REGISTER gdtr = { 0 };
        _DESCRIPTOR_REGISTER idtr = { 0 };

        _sgdt(&gdtr);
        __sidt(&idtr);

        
        auto* control = &currentProcessor->guestVmcb.controlArea;
        auto* state = &currentProcessor->guestVmcb.stateSaveArea;
        auto* cs = &state->cs;
        auto* ds = &state->ds;
        auto* es = &state->es;
        auto* ss = &state->ss;
        

        const auto hostStateAreaPa = MmGetPhysicalAddress(&currentProcessor->hostStateArea).QuadPart;
        const auto sharedPagesPa = MmGetPhysicalAddress(&this->nestedTables->pml4e).QuadPart;
      

        control->vector4.bit.vmRun = 1;
        control->vector4.bit.vmLoad = 1;
        control->vector4.bit.vmSave = 1;
        control->vector4.bit.vmCall = 1;
        control->vector4.bit.xsetbv = 1;
        control->vector4.bit.rdtscp = 1;
        control->vector4.bit.clgi = 1;
        control->vector4.bit.skinit = 1;
        control->vector4.bit.stgi = 1;
        control->vector3.bit.msrProto = 1;
        control->vector3.bit.cpuid = 1;
        control->vector3.bit.rdtsc = 1;

        
        currentProcessor->hostStack.layout.xSetBvMask = this->xCr0BitMask;
        currentProcessor->hostStack.layout.eferBitMask = this->eferBitMask;
        control->guestAsidTlbControl.value = 1;
        control->npEnable.value |= SVM_NP_ENABLE;
        control->nCr3 = sharedPagesPa;

        state->gdtr.base = gdtr.base;
        state->gdtr.limit = gdtr.limit;
        state->idtr.base = idtr.base;
        state->idtr.limit = idtr.limit;

        cs->selector.value = ReadCs();
        ds->selector.value = ReadDs();
        es->selector.value = ReadEs();
        ss->selector.value = ReadSs();

        cs->limit = __segmentlimit(cs->selector.value);
        ds->limit = __segmentlimit(ds->selector.value);
        es->limit = __segmentlimit(es->selector.value);
        ss->limit = __segmentlimit(ss->selector.value);

     
        cs->FetchAtts(gdtr.base);
        ds->FetchAtts(gdtr.base);
        es->FetchAtts(gdtr.base);
        ss->FetchAtts(gdtr.base);

        state->efer.ReadValue();
        state->cr0.value = __readcr0();
        state->cr2 = __readcr2();
        state->cr3.value = __readcr3();
        state->cr4.value = __readcr4();
        state->rflags.value = __getcallerseflags();
        state->rsp = ReadRsp();
        state->rip = ReadRip();
        state->dbgCtl = __readmsr(MSR_DBG_CTL);
        state->gPat = __readmsr(MSR_PAT);
        control->vmcbClean.value = 0;
        
        __svm_vmsave(currentProcessor->hostStack.layout.guestVmcbPa);

        currentProcessor->hostStack.layout.hvProcessor = currentProcessor;

        __writemsr(SVM_MSR_VM_HSAVE_PA, hostStateAreaPa);

        __svm_vmsave(currentProcessor->hostStack.layout.hostVmcbPa);
    }




    NTSTATUS Hypervisor::VirtualizeProcessor(__in const uint32_t processorIndex)
    {    
        NTSTATUS status = -1;
        auto currentProcessor = static_cast<_HV_CPU*>(Inlines::poolAlloc(sizeof(_HV_CPU)));
        if (!currentProcessor)
			return STATUS_NO_MEMORY;
       
        if (!this->VirtulizationCheck(processorIndex))
        {

            currentProcessor->hostStack.layout.guestVmcbPa = MmGetPhysicalAddress(&currentProcessor->guestVmcb).QuadPart;
            currentProcessor->hostStack.layout.hostVmcbPa = MmGetPhysicalAddress(&currentProcessor->hostVmcb).QuadPart;
            currentProcessor->guestVmcb.controlArea.msrpmBasePa = MmGetPhysicalAddress(this->msrMap).QuadPart;
            currentProcessor->hostStack.layout.nestedPages = this->nestedTables;
            
            
            __writemsr(MSR_EFER, __readmsr(MSR_EFER) | EFER_SVME);

            KPRINT("[+]Setting Up Virtual Machine Block\n");

            this->BuildOutVmcb(currentProcessor);

            if (!NT_SUCCESS(status = this->SetupHostDetals(currentProcessor)))
            {
                KPRINT("[-]Host Details Setup Failed\n");
				return status;
            }

            KPRINT("[+]Entering Virtual Machine\n");

            VmEnter(&currentProcessor->hostStack.layout.guestVmcbPa);

            KeBugCheck(MANUALLY_INITIATED_CRASH);
        }

        KPRINT("[+]Processor Virtualized\n");
      
        return STATUS_SUCCESS;
    }




    NTSTATUS Hypervisor::RunProcessors()
    {
        NTSTATUS status = -1;
        uint32_t i = 0;
        PROCESSOR_NUMBER pNumber = { 0 };
        GROUP_AFFINITY groupAffinity = { 0 };
        GROUP_AFFINITY dwAffinity = { 0 };

        auto processorCount = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);
        
        for (; i < processorCount; i++)
        {   

            if (!NT_SUCCESS(status = KeGetProcessorNumberFromIndex(i, &pNumber)))
			{
				KPRINT("[-]Processor Number Fetch Failed\n");
				break;
			}

            Inlines::memSet(&groupAffinity, 0, sizeof(GROUP_AFFINITY));
            groupAffinity.Group = pNumber.Group;
            groupAffinity.Mask = 1ULL << pNumber.Number;
            
            KeSetSystemGroupAffinityThread(&groupAffinity, &dwAffinity);
          

            auto dwIrql = KeRaiseIrqlToDpcLevel();
                    
            status = this->VirtualizeProcessor(i);

            KeLowerIrql(dwIrql);

            KeRevertToUserGroupAffinityThread(&dwAffinity);

            if (!NT_SUCCESS(status))
                break;
        }

        if (!NT_SUCCESS(status) || i != processorCount)
		{
			KPRINT("[-]Processor Virtualization Failed\n");
			return STATUS_UNEXPECTED_IO_ERROR;
		}


        return STATUS_SUCCESS;
    }



    void Hypervisor::StartVm()
    {
   
        KPRINT("[+]Nested Pages Setup\n");
        this->nestedTables = static_cast<_SHARED_PAGES*>(Inlines::poolAlloc(sizeof(_SHARED_PAGES)));
        if (!this->nestedTables)
            return;
         
        this->SetupTables(this->nestedTables, false);
        KPRINT("[+]Nested Pages Setup Success\n");

       
        
        KPRINT("[+]MSR Map Setup\n");     
        this->msrMap = Inlines::contigAlloc(SVM_MSR_PERMISSIONS_MAP_SIZE);
        if (!this->msrMap)
        {
            Inlines::poolFree(this->nestedTables);
            return;
        }
        KPRINT("[+]MSR Map Allocated\n");

       
        MsrSetup(this->msrMap);
        KPRINT("[+]MSR Map Setup Success\n");

        
        KPRINT("[+]Fetching Efer Register Bit Mask\n");
        this->eferBitMask.ReadValue();

        
        KPRINT("[+]Fetching Xcr0 Register Bit Mask\n");
        this->xCr0BitMask.ReadValue();
        
        
        KPRINT("[+]Hypervisor Setup\n");      
        NTSTATUS status = -1; 
        status = this->RunProcessors();
        if (!NT_SUCCESS(status))
            KeBugCheck(MANUALLY_INITIATED_CRASH);
           
    }



    void Hypervisor::SetupTables(__inout _SHARED_PAGES* sharedPages, __in const bool host) const
    {
   	  auto* pages = sharedPages;
        
        const auto pdpBasePa = MmGetPhysicalAddress(&pages->pdpe).QuadPart;
        pages->pml4e[0].bit.pageFrameNumber = pdpBasePa >> PAGE_SHIFT;
        pages->pml4e[0].bit.valid = 1;
        pages->pml4e[0].bit.write = 1;
        pages->pml4e[0].bit.user = 1;

        for (uintmax_t pdpIndex = 0; pdpIndex < TABLE_PAGE_SIZE; pdpIndex++)
        {
  
            const auto pdeBasePa = MmGetPhysicalAddress(&pages->pde[pdpIndex][0]).QuadPart;
            pages->pdpe[pdpIndex].bit.pageFrameNumber = pdeBasePa >> PAGE_SHIFT;
            pages->pdpe[pdpIndex].bit.valid = 1;
            pages->pdpe[pdpIndex].bit.write = 1;
            pages->pdpe[pdpIndex].bit.user = 1;

            for (uintmax_t pdIndex = 0; pdIndex < TABLE_PAGE_SIZE; pdIndex++)
            {
                const auto transAddr = (pdpIndex << 9) ^ pdIndex;
                pages->pde[pdpIndex][pdIndex].bit.pageFrameNumber = transAddr;
                pages->pde[pdpIndex][pdIndex].bit.valid = 1;
                pages->pde[pdpIndex][pdIndex].bit.write = 1;
                pages->pde[pdpIndex][pdIndex].bit.user = 1;
                pages->pde[pdpIndex][pdIndex].bit.largePage = 1;
            }
        }



        if (host)
        {
            PHYSICAL_ADDRESS pml4BasePa = { 0 };

            if (this->setupFlags.bit.pcideEnabled)
            {
                pml4BasePa.QuadPart = this->systemCr3.pcideOn.pml4 << 12;
            }
            else
            pml4BasePa.QuadPart = this->systemCr3.pml4 << 12;
            
            
            const auto pml4BaseVa = reinterpret_cast<_PML4_ENTRY_2MB*>(MmGetVirtualForPhysical(pml4BasePa));
            Inlines::memCpy(&pages->pml4e[256], &pml4BaseVa[256], sizeof(_PML4_ENTRY_2MB) << 8);

            KPRINT("[+]Host Pages Setup\n");
        }
       
    }



    NTSTATUS Hypervisor::SetupHostDetals(__inout _HV_CPU* currentProcessor)
    {
        
        this->systemProcess = reinterpret_cast<__EPROCESS*>(PsInitialSystemProcess);
        if (!this->systemProcess)
			return STATUS_UNSUCCESSFUL;

        
        this->systemCr3.value = this->systemProcess->Pcb.DirectoryTableBase;
        
        
        this->hostPages = static_cast<_SHARED_PAGES*>(Inlines::poolAlloc(sizeof(_SHARED_PAGES)));
        if (!this->hostPages)
			return STATUS_NO_MEMORY;

        
        this->SetupTables(this->hostPages, true);

        currentProcessor->hostStack.layout.hostPages = this->hostPages;

        currentProcessor->hostVmcb.stateSaveArea.cr3.value = 0;

        if (this->setupFlags.bit.pcideEnabled)
        {
            currentProcessor->hostVmcb.stateSaveArea.cr3.pcideOn.pml4 = MmGetPhysicalAddress(&this->hostPages->pml4e).QuadPart >> 12;
        }			
		else
        currentProcessor->hostVmcb.stateSaveArea.cr3.pml4 = MmGetPhysicalAddress(&this->hostPages->pml4e).QuadPart >> 12;

        __writecr3(currentProcessor->hostVmcb.stateSaveArea.cr3.value);

		return STATUS_SUCCESS;
    }



    void Hypervisor::CheckForSetupFlags()
    {
       int32_t registers[4] = { -1 };
       _CR0_REGISTER cr0{};
       cr0.value = __readcr0();
       _CR4_REGISTER cr4{};
       cr4.value = __readcr4();
       _RFLAGS_REGISTER rFlags{};
       rFlags.value = __getcallerseflags();

       
       
        // eax ebx ecx edx
        __cpuid(registers, CPUID_PROCESSOR_AND_PROCESSOR_FEATURE_IDENTIFIERS);
        if ((registers[2] & CPUID_FN0000_0001_ECX_X2APIC_MODE))
        {
            Hypervisor::setupFlags.value |= X2APIC_ENABLED;
            KPRINT("[+]X2APIC Mode Enabled\n");
        }
        else if ((registers[3] & CPUID_FN0000_0001_EDX_APIC_MODE))
        {
            Hypervisor::setupFlags.value |= APIC_ENABLED;
            KPRINT("[+]APIC Mode Enabled\n");
        }
          
        
        if (cr4.bit.pcide)
        {
            this->setupFlags.value |= PCIDE_ENABLED;
            KPRINT("[+]PCIDE Enabled\n");
        }

        if (cr4.bit.la57)
        {
            this->setupFlags.value |= LEVEL5_PAGING;
            KPRINT("[+]Level 5 Paging Enabled\n");
        }


        if (rFlags.bit.virtual8086)
        {  
            KPRINT("[+Virtual 8086 Mode enabled\n")
            return;
        }

        if (cr0.bit.pg && cr0.bit.pe)
		{
			this->setupFlags.value |= SHADOW_STACK_OPERATIONAL;
            KPRINT("[+]Shadow Stack Operational\n");
		}
         
        if (cr4.bit.cet)
        {
            this->setupFlags.value |= SHADOW_STACK_ENABLED;
            KPRINT("[+]Shadow Stack Enabled\n");
        } 

    }


    NTSTATUS Hypervisor::CheckSupport()
    {
        int32_t registers[4] = { -1 };


        // eax ebx ecx edx
        __cpuid(registers, CPUID_MAX_STANDARD_FN_NUMBER_AND_VENDOR_STRING);
        if ((registers[1] != 'htuA') ||
            (registers[3] != 'itne') ||
            (registers[2] != 'DMAc'))
            return STATUS_HV_INVALID_DEVICE_ID;


        __cpuid(registers, CPUID_PROCESSOR_AND_PROCESSOR_FEATURE_IDENTIFIERS_EX);
        if (!(registers[2] & CPUID_FN8000_0001_ECX_SVM))
            return STATUS_HV_CPUID_FEATURE_VALIDATION_ERROR;


        if (!(registers[3] & CPUID_FN8000_0001_EDX_LONG_MODE))
            return STATUS_HV_CPUID_FEATURE_VALIDATION_ERROR;


        __cpuid(registers, CPUID_SVM_FEATURES);
        if (!(registers[3] & CPUID_FN8000_000A_EDX_NP))
            return STATUS_HV_CPUID_FEATURE_VALIDATION_ERROR;


        const auto vmcr = __readmsr(SVM_MSR_VM_CR);
        if (vmcr & SVM_VM_CR_SVMDIS)
            return STATUS_HV_INVALID_DEVICE_STATE;


        KPRINT("[+]Support Success\n");

        return STATUS_SUCCESS;
    }


    void Hypervisor::MsrSetup(__in void* msrPermsAddr)
    {

        RTL_BITMAP msrBitmap = { 0 };
        constexpr uint16_t bitsPerMsr = 0x02;
        constexpr uintmax_t msrRange = 0xC0000000;
        constexpr uint16_t szVector = (0x800 * CHAR_BIT);
        constexpr uintmax_t registerOffset = (MSR_EFER - msrRange) * bitsPerMsr;
        constexpr uintmax_t offset = registerOffset + szVector;

        RtlInitializeBitMap(&msrBitmap, static_cast<PULONG>(msrPermsAddr), (SVM_MSR_PERMISSIONS_MAP_SIZE * CHAR_BIT));
        RtlClearAllBits(&msrBitmap);
        RtlSetBits(&msrBitmap, (offset + 0x01UL), 0x01UL);
    }



    bool Hypervisor::VirtulizationCheck(__in const uint32_t& processorIndex)
    {

        if (this->ProcessorVirtualized[processorIndex]) return true;

        this->ProcessorVirtualized[processorIndex] = true;

        return false;
    }
