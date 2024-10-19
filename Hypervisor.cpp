#include <ntifs.h>

#include "Hypervisor.hpp"
#include "CustomFunctions.hpp"


/**
 * @brief Builds the Virtual Machine Control Block (VMCB) for the current processor.
 *
 * @param currentProcessor Pointer to the current processor's hypervisor CPU structure.
 * @param context Pointer to the guest context structure containing necessary information.
 */
void Hypervisor::BuildOutVmcb( __inout _HV_CPU* currentProcessor, __inout const _GUEST_CONTEXT* context )
{
    // Define pointers to different areas of the VMCB
    auto* control = &currentProcessor->guestVmcb.controlArea;
    auto* state = &currentProcessor->guestVmcb.stateSaveArea;
    auto* cs = &state->cs;
    auto* ds = &state->ds;
    auto* es = &state->es;
    auto* ss = &state->ss;

    // Get physical addresses for host state area, nested tables, guest VMCB, host VMCB, and MSR map
    const auto hostStateAreaPa = MmGetPhysicalAddress( &currentProcessor->hostStateArea ).QuadPart;
    const auto sharedPagesPa = MmGetPhysicalAddress( &Hypervisor::nestedTables->pml4e ).QuadPart;
    const auto guestPa = MmGetPhysicalAddress( &currentProcessor->guestVmcb ).QuadPart;
    const auto hostPa = MmGetPhysicalAddress( &currentProcessor->hostVmcb ).QuadPart;
    const auto msrPa = MmGetPhysicalAddress( Hypervisor::nestedTables->msrMap ).QuadPart;

    // Set control flags for the guest VMCB
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

    // Set guest ASID and nested page related fields
    control->guestAsid.value = 1;
    control->npEnable.value |= SVM_NP_ENABLE;
    control->nCr3 = sharedPagesPa;
    control->msrpmBasePa = msrPa;

    // Load guest descriptor tables from the provided context
    state->gdtr.base = context->gdtr.base;
    state->gdtr.limit = context->gdtr.limit;
    state->idtr.base = context->idtr.base;
    state->idtr.limit = context->idtr.limit;

    // Load segment selectors from the provided context
    cs->selector.value = context->cs;
    ds->selector.value = context->ds;
    es->selector.value = context->es;
    ss->selector.value = context->ss;

    // Set segment limits based on context
    cs->limit = static_cast< uint32_t >( __segmentlimit( context->cs ) );
    ds->limit = static_cast< uint32_t >( __segmentlimit( context->ds ) );
    es->limit = static_cast< uint32_t >( __segmentlimit( context->es ) );
    ss->limit = static_cast< uint32_t >( __segmentlimit( context->ss ) );

    // Fetch segment attributes based on GDTR base
    cs->FetchAtts( context->gdtr.base );
    ds->FetchAtts( context->gdtr.base );
    es->FetchAtts( context->gdtr.base );
    ss->FetchAtts( context->gdtr.base );

    // Load the remaining guest state from the provided context
    state->efer = context->efer;
    state->cr0.value = context->cr0;
    state->cr2 = context->cr2;
    state->cr3.value = context->cr3;
    state->cr4.value = context->cr4;
    state->rflags.value = context->rflags;
    state->rsp = context->rsp;
    state->rip = context->rip;
    state->gPat = context->gPat;

    // Save the guest VMCB state
    __svm_vmsave( guestPa );

    // Set up the host stack layout for VMCB information
    currentProcessor->hostStack.layout.reserved0 = MAXUINT64;
    currentProcessor->hostStack.layout.xSetBvMask = Hypervisor::xCr0BitMask;
    currentProcessor->hostStack.layout.nestedPages = Hypervisor::nestedTables;
    currentProcessor->hostStack.layout.hvProcessor = currentProcessor;
    currentProcessor->hostStack.layout.hostVmcbPa = hostPa;
    currentProcessor->hostStack.layout.guestVmcbPa = guestPa;

    // Write the host state physical address to the MSR
    __writemsr( SVM_MSR_VM_HSAVE_PA, hostStateAreaPa );

    // Save the host VMCB state
    __svm_vmsave( hostPa );
}


/**
 * @brief Virtualizes the current processor using the hypervisor.
 *
 * Initializes guest state and sets up the environment for virtualization.
 *
 * @return NTSTATUS indicating the success or failure of the operation.
 */
NTSTATUS Hypervisor::VirtualizeProcessor()
{
    _GUEST_CONTEXT state = { 0 };
    NTSTATUS status = -1;

#ifndef NESTED
    auto* currentProcessor = &Hypervisor::processors[ KeGetCurrentProcessorNumberEx( nullptr ) ];
#else
    auto* currentProcessor = static_cast< _HV_CPU* >( Inlines::poolAlloc( sizeof( _HV_CPU ) ) );
    if ( !currentProcessor )
        return STATUS_NO_MEMORY;
#endif // !NESTED

    // Read and update VM_CR and EFER registers
    _MSR_VM_CR vmCr{};
    vmCr.ReadValue();
    if ( vmCr.svmeDisable() )
    {
        vmCr.LockAndWrite();
    }

    _EFER_REGISTER efer{};
    efer.ReadValue();
    if ( !efer.bit.svme )
    {
        efer.SetSvmeBit();
        efer.WriteValue();
    }

    // Capture the current guest context
    CAPTURE_GUEST( &state );

#ifdef NESTED
    if ( !Hypervisor::HyperVirtulizationCheck() )
    {
        // Build VMCB and launch the guest virtual machine
        Hypervisor::BuildOutVmcb( currentProcessor, &state );
        LaunchVm( &currentProcessor->hostStack.layout.guestVmcbPa );

        // Should never be reached - indicates an error
        DEBUG_BREAK();
        KeBugCheck( MANUALLY_INITIATED_CRASH );
    }
#else
    if ( !Hypervisor::VirtulizationCheck() )
    {
        // Build VMCB and launch the guest virtual machine
        Hypervisor::BuildOutVmcb( currentProcessor, &state );
        LaunchVm( &currentProcessor->hostStack.layout.guestVmcbPa );

        // Should never be reached - indicates an error
        DEBUG_BREAK();
        KeBugCheck( MANUALLY_INITIATED_CRASH );
    }
#endif

    KPRINT( "[+]Processor Virtualized\n" );
    return STATUS_SUCCESS;
}

/**
 * @brief DPC routine to virtualize the current processor.
 *
 * @param dpc Pointer to the DPC object.
 * @param deferredContext Context pointer passed to the DPC.
 * @param systemArg1 System argument 1.
 * @param systemArg2 System argument 2.
 */
void VirtualizeProcessorDPCRoutine( __in PKDPC dpc, __in_opt void* deferredContext, __in_opt void* systemArg1, __in_opt void* systemArg2 )
{
    auto* asyncContext = static_cast< DPC_CONTEXT* >( deferredContext );
    auto currentProcessor = KeGetCurrentProcessorNumberEx( nullptr );
    auto processorCount = KeQueryActiveProcessorCountEx( ALL_PROCESSOR_GROUPS );

    // Call the virtualization function if provided
    if ( asyncContext->virtualizeFunction != nullptr && asyncContext->context != nullptr )
    {
        auto status = asyncContext->virtualizeFunction( asyncContext->context );
        asyncContext->status[ currentProcessor ] = status;
    }

    // Increment the count of processors virtualized
    InterlockedIncrement( &asyncContext->count );

    if ( systemArg1 != nullptr && systemArg2 != nullptr )
    {
        KeSignalCallDpcSynchronize( systemArg2 );
        KeSignalCallDpcDone( systemArg1 );
    }

    // Signal event if all processors have been virtualized
    if ( asyncContext->count == processorCount )
    {
        KeSetEvent( &asyncContext->event, IO_NO_INCREMENT, FALSE );
    }
}


/**
 * @brief Starts virtualization for all active processors.
 *
 * This function will iterate through all active processors and start virtualization on each one.
 *
 * @return NTSTATUS indicating the success or failure of the operation.
 */
NTSTATUS Hypervisor::RunProcessors()
{
    // Create a lambda function for virtualizing the processor
    auto virtualizeProcessor = []( void* context ) -> NTSTATUS
        {
            return static_cast< Hypervisor* >( context )->VirtualizeProcessor();
        };

    NTSTATUS status = STATUS_UNSATISFIED_DEPENDENCIES;
    KDPC dpc = { 0 };
    DPC_CONTEXT asyncContext = { 0 };

    auto processorCount = KeQueryActiveProcessorCountEx( ALL_PROCESSOR_GROUPS );

    asyncContext.status = static_cast< NTSTATUS* >( Inlines::poolAlloc( ( processorCount * sizeof( NTSTATUS ) ) ) );
    if ( !asyncContext.status )
        return STATUS_NO_MEMORY;

    asyncContext.count = 0;
    asyncContext.context = this;
    asyncContext.virtualizeFunction = virtualizeProcessor;
    asyncContext.deVirtualizeFunction = nullptr;

    KPRINT( "[+]Active Processor Count: %d\n", processorCount );

    // Initialize an event for synchronization
    KeInitializeEvent( &asyncContext.event, NotificationEvent, FALSE );

    // Call the DPC routine to virtualize processors
    KeGenericCallDpc( VirtualizeProcessorDPCRoutine, &asyncContext );

    // Wait for all processors to be virtualized
    KeWaitForSingleObject( &asyncContext.event, Executive, KernelMode, FALSE, nullptr );

    // Check the status of each processor's virtualization
    for ( auto i = 0; i < processorCount; i++ )
    {
        if ( !NT_SUCCESS( asyncContext.status[ i ] ) )
        {
            status = asyncContext.status[ i ];
            KPRINT( "[-]Processor %d Virtualization Failed\n", i );
        }
    }

    // Free allocated memory for processor status
    if ( asyncContext.status != nullptr )
    {
        Inlines::poolFree( asyncContext.status );
        asyncContext.status = nullptr;
        asyncContext.context = nullptr;
    }

    if ( status == STATUS_UNSATISFIED_DEPENDENCIES )
    {
        status = STATUS_SUCCESS;
    }

    if ( !NT_SUCCESS( status ) )
    {
        KPRINT( "[-]Hypervisor Virtualization Failed\n" );
        return status;
    } else
    {
        KPRINT( "[+]Hypervisor Virtualized\n" );
        return status;
    }

    return status;
}

/**
 * @brief Starts the virtual machine by allocating necessary resources and setting up nested pages.
 *
 * @return NTSTATUS indicating the success or failure of the operation.
 */
NTSTATUS Hypervisor::StartVm()
{
    NTSTATUS status = STATUS_UNSATISFIED_DEPENDENCIES;

#ifndef NESTED
    // Allocate memory for processors' hypervisor structures
    Hypervisor::processors = static_cast< _HV_CPU* >( Inlines::poolAlloc( sizeof( _HV_CPU ) * KeQueryActiveProcessorCountEx( ALL_PROCESSOR_GROUPS ) ) );
    if ( !Hypervisor::processors )
        return STATUS_NO_MEMORY;
#endif // !NESTED

    // Allocate nested page structures
    KPRINT( "[+]Nested Pages Setup\n" );
    Hypervisor::nestedTables = static_cast< _SHARED_PAGES* >( Inlines::poolAlloc( sizeof( _SHARED_PAGES ) ) );
    if ( !Hypervisor::nestedTables )
        goto end;

    Hypervisor::SetupTables( Hypervisor::nestedTables );
    KPRINT( "[+]Nested Pages Setup Success\n" );

    // Allocate MSR permission map and set up MSR permissions
    KPRINT( "[+]MSR Map Setup\n" );
    Hypervisor::nestedTables->msrMap = Inlines::contigAlloc( SVM_MSR_PERMISSIONS_MAP_SIZE );
    if ( !Hypervisor::nestedTables->msrMap )
        goto end;

    MsrSetup( Hypervisor::nestedTables->msrMap );
    KPRINT( "[+]MSR Map Setup Success\n" );

    // Fetch the Xcr0 bitmask for SVM
    KPRINT( "[+]Fetching Xcr0 Register Bit Mask\n" );
    Hypervisor::xCr0BitMask.ReadValue();

    // Run processors to start virtualization
    status = Hypervisor::RunProcessors();

    if ( !NT_SUCCESS( status ) )
        goto end;

    KPRINT( "[+]Hypervisor Started\n" );

    return STATUS_SUCCESS;

end:
    // Free allocated resources if setup failed
    if ( Hypervisor::nestedTables->msrMap != nullptr )
    {
        Inlines::contigFree( Hypervisor::nestedTables->msrMap, SVM_MSR_PERMISSIONS_MAP_SIZE );
        Hypervisor::nestedTables->msrMap = nullptr;
    }
    if ( Hypervisor::nestedTables != nullptr )
    {
        Inlines::poolFree( Hypervisor::nestedTables );
        Hypervisor::nestedTables = nullptr;
    }

#ifndef NESTED
    if ( Hypervisor::processors != nullptr )
    {
        Inlines::poolFree( Hypervisor::processors );
        Hypervisor::processors = nullptr;
    }
#endif // !NESTED

    return status;
}

/**
 * @brief Sets up the nested page tables for the hypervisor.
 *
 * @param sharedPages Pointer to shared page structures.
 */
void Hypervisor::SetupTables( __inout _SHARED_PAGES* sharedPages ) const
{
    auto* pages = sharedPages;

    // Set up PML4 entry for the nested page tables
    const auto pdpBasePa = MmGetPhysicalAddress( &pages->pdpe ).QuadPart;
    pages->pml4e[ 0 ].bit.pageFrameNumber = pdpBasePa >> PAGE_SHIFT;
    pages->pml4e[ 0 ].bit.valid = 1;
    pages->pml4e[ 0 ].bit.write = 1;
    pages->pml4e[ 0 ].bit.user = 1;

    // Set up the PDPE entries for the nested page tables
    for ( uintmax_t pdpIndex = 0; pdpIndex < TABLE_PAGE_SIZE; pdpIndex++ )
    {
        const auto pdeBasePa = MmGetPhysicalAddress( &pages->pde[ pdpIndex ][ 0 ] ).QuadPart;
        pages->pdpe[ pdpIndex ].bit.pageFrameNumber = pdeBasePa >> PAGE_SHIFT;
        pages->pdpe[ pdpIndex ].bit.valid = 1;
        pages->pdpe[ pdpIndex ].bit.write = 1;
        pages->pdpe[ pdpIndex ].bit.user = 1;

        // Set up the PDE entries for the nested page tables
        for ( uintmax_t pdIndex = 0; pdIndex < TABLE_PAGE_SIZE; pdIndex++ )
        {
            const auto transAddr = ( pdpIndex << 9 ) ^ pdIndex;
            pages->pde[ pdpIndex ][ pdIndex ].bit.pageFrameNumber = transAddr;
            pages->pde[ pdpIndex ][ pdIndex ].bit.valid = 1;
            pages->pde[ pdpIndex ][ pdIndex ].bit.write = 1;
            pages->pde[ pdpIndex ][ pdIndex ].bit.user = 1;
            pages->pde[ pdpIndex ][ pdIndex ].bit.largePage = 1;
        }
    }
}

/**
 * @brief Checks and sets up flags for the hypervisor setup.
 *
 * Detects various features like x2APIC mode, PCIDE, Level 5 paging, etc.
 */
void Hypervisor::CheckForSetupFlags()
{
    int32_t registers[ 4 ] = { -1 };
    _CR0_REGISTER cr0{};
    cr0.value = __readcr0();
    _CR4_REGISTER cr4{};
    cr4.value = __readcr4();
    _RFLAGS_REGISTER rFlags{};
    rFlags.value = __getcallerseflags();

    // eax ebx ecx edx
    __cpuid( registers, CPUID_PROCESSOR_AND_PROCESSOR_FEATURE_IDENTIFIERS );
    if ( ( registers[ 2 ] & CPUID_FN0000_0001_ECX_X2APIC_MODE ) )
    {
        Hypervisor::setupFlags.value |= X2APIC_ENABLED;
        KPRINT( "[+]X2APIC Mode Enabled\n" );
    }

    if ( cr4.bit.pcide )
    {
        Hypervisor::setupFlags.value |= PCIDE_ENABLED;
        KPRINT( "[+]PCIDE Enabled\n" );
    }

    if ( cr4.bit.la57 )
    {
        Hypervisor::setupFlags.value |= LEVEL5_PAGING;
        KPRINT( "[+]Level 5 Paging Enabled\n" );
    }

    if ( rFlags.bit.virtual8086 )
    {
        KPRINT( "[+Virtual 8086 Mode enabled\n" );
        return;
    }

    if ( cr0.bit.pg && cr0.bit.pe )
    {
        Hypervisor::setupFlags.value |= SHADOW_STACK_OPERATIONAL;
        KPRINT( "[+]Shadow Stack Operational\n" );
    }
}

/**
 * @brief Checks for hardware support required for hypervisor setup.
 *
 * Ensures that the CPU and system meet the requirements to run the hypervisor.
 *
 * @return NTSTATUS indicating the success or failure of the operation.
 */
NTSTATUS Hypervisor::CheckSupport()
{
    int32_t registers[ 4 ] = { -1 };

    // eax ebx ecx edx
    __cpuid( registers, CPUID_MAX_STANDARD_FN_NUMBER_AND_VENDOR_STRING );
    if ( ( registers[ 1 ] != 'htuA' ) ||
         ( registers[ 3 ] != 'itne' ) ||
         ( registers[ 2 ] != 'DMAc' ) )
        return STATUS_HV_INVALID_DEVICE_ID;

    __cpuid( registers, CPUID_PROCESSOR_AND_PROCESSOR_FEATURE_IDENTIFIERS_EX );
    if ( !( registers[ 2 ] & CPUID_FN8000_0001_ECX_SVM ) )
        return STATUS_HV_CPUID_FEATURE_VALIDATION_ERROR;

    if ( !( registers[ 3 ] & CPUID_FN8000_0001_EDX_LONG_MODE ) )
        return STATUS_HV_CPUID_FEATURE_VALIDATION_ERROR;

    __cpuid( registers, CPUID_SVM_FEATURES );
    if ( !( registers[ 3 ] & CPUID_FN8000_000A_EDX_NP ) )
        return STATUS_HV_CPUID_FEATURE_VALIDATION_ERROR;

    const auto vmcr = __readmsr( SVM_MSR_VM_CR );
    if ( vmcr & SVM_VM_CR_SVMDIS )
        return STATUS_HV_INVALID_DEVICE_STATE;

    KPRINT( "[+]Support Success\n" );

    return STATUS_SUCCESS;
}

/**
 * @brief Sets up MSR permissions for the hypervisor.
 *
 * @param msrPermsAddr Address of the MSR permission bitmap.
 */
void Hypervisor::MsrSetup( __inout void* msrPermsAddr )
{
    RTL_BITMAP msrBitmap = { 0 };
    constexpr uint16_t bitsPerMsr = 0x02;
    constexpr uintmax_t msrRange = 0xC0000000;
    constexpr uint16_t szVector = ( 0x800 * CHAR_BIT );
    constexpr uintmax_t registerOffset = ( MSR_EFER - msrRange ) * bitsPerMsr;
    constexpr uintmax_t offset = registerOffset + szVector;

    // Initialize the MSR bitmap and set specific bits
    RtlInitializeBitMap( &msrBitmap, static_cast< PULONG >( msrPermsAddr ), ( SVM_MSR_PERMISSIONS_MAP_SIZE * CHAR_BIT ) );
    RtlClearAllBits( &msrBitmap );
    RtlSetBits( &msrBitmap, ( offset + 0x01UL ), 0x01UL );
}

#ifdef NESTED
/**
 * @brief Checks if hyper-virtualization is present.
 *
 * @return true if hyper-virtualization is detected, false otherwise.
 */
__declspec( noinline ) bool Hypervisor::HyperVirtulizationCheck()
{
    int32_t registers[ 4 ] = { -1 };
    char vendor[ 13 ] = { 0 };
    vendor[ 12 ] = '\0';

    // Get hypervisor vendor string
    __cpuid( registers, CPUID_HV_VENDOR_AND_MAX_FUNCTIONS );

    Inlines::memCpy( vendor + 0, &registers[ 1 ], sizeof( registers[ 1 ] ) );
    Inlines::memCpy( vendor + 4, &registers[ 2 ], sizeof( registers[ 2 ] ) );
    Inlines::memCpy( vendor + 8, &registers[ 3 ], sizeof( registers[ 3 ] ) );

    // Check if the vendor matches "IceCoaled"
    if ( strcmp( vendor, "IceCoaled   " ) != 0 ) return false;

    return true;
}
#else

/**
 * @brief Checks if the processor has already been virtualized.
 *
 * @return true if the processor has already been virtualized, false otherwise.
 */
__declspec( noinline ) bool Hypervisor::VirtulizationCheck()
{
    const auto processorIndex = KeGetCurrentProcessorNumberEx( nullptr );

    if ( !Hypervisor::ProcessorVirtualized[ processorIndex ] )
    {
        Hypervisor::ProcessorVirtualized[ processorIndex ] = true;
        return false;
    }

    return true;
}
#endif // NESTED

    
#ifdef NESTED
/**
 * @brief Devirtualizes the current processor in a nested virtualization scenario.
 *
 * Uses CPUID to verify the hypervisor vendor before devirtualizing.
 *
 * @return NTSTATUS indicating the success of the devirtualization process.
 */
NTSTATUS Hypervisor::DevirtualizeProcessor()
{
    // eax ebx ecx edx
    int32_t registers[ 4 ];
    uintmax_t high = 0;
    uintmax_t low = 0;
    _HV_CPU* currentProcessor = nullptr;

    // Get the CPU information to verify the hypervisor vendor
    __cpuidex( registers, CPUID_UNLOAD, CPUID_UNLOAD );
    if ( registers[ 2 ] != 'Ice' )
    {
        goto exit;
    }

    KPRINT( "The processor has been Devirtualized\n" );

    // Combine high and low values to get the current processor pointer
    high = registers[ 3 ];
    low = registers[ 0 ] & MAXUINT32;
    currentProcessor = reinterpret_cast< _HV_CPU* >( high << 32 | low );
    NT_ASSERT( vpData->HostStackLayout.Reserved1 == MAXUINT64 );

    // Free the memory allocated for the current processor
    Inlines::poolFree( currentProcessor );
    currentProcessor = nullptr;

exit:
    return STATUS_SUCCESS;
}

/**
 * @brief DPC routine to devirtualize the current processor.
 *
 * @param dpc Pointer to the DPC object.
 * @param deferredContext Context pointer passed to the DPC.
 * @param systemArg1 System argument 1.
 * @param systemArg2 System argument 2.
 */
void DevirtualizeProcessorDPCRoutine( __in PKDPC dpc, __in_opt void* deferredContext, __in_opt void* systemArg1, __in_opt void* systemArg2 )
{
    auto* asyncContext = static_cast< DPC_CONTEXT* >( deferredContext );
    auto currentProcessor = KeGetCurrentProcessorNumberEx( nullptr );
    auto processorCount = KeQueryActiveProcessorCountEx( ALL_PROCESSOR_GROUPS );

    // Call the devirtualization function if provided
    if ( asyncContext->deVirtualizeFunction != nullptr &&
         asyncContext->context != nullptr )
    {
        auto status = asyncContext->deVirtualizeFunction( asyncContext->context );
        asyncContext->status[ currentProcessor ] = status;
    }

    // Increment the count of processors devirtualized
    InterlockedIncrement( &asyncContext->count );

    if ( systemArg1 != nullptr &&
         systemArg2 != nullptr )
    {
        KeSignalCallDpcSynchronize( systemArg2 );
        KeSignalCallDpcDone( systemArg1 );
    }

    // Signal event if all processors have been devirtualized
    if ( asyncContext->count == processorCount )
    {
        KeSetEvent( &asyncContext->event, IO_NO_INCREMENT, FALSE );
    }
}

/**
 * @brief Starts the devirtualization process for all active processors.
 *
 * This function will iterate through all active processors and initiate devirtualization on each one.
 *
 * @return NTSTATUS indicating the success or failure of the operation.
 */
NTSTATUS Hypervisor::Devirtualize()
{
    // Create a lambda function for devirtualizing the processor
    auto devirtualizeProcessor = []( void* context ) -> NTSTATUS
        {
            return static_cast< Hypervisor* >( context )->DevirtualizeProcessor();
        };

    NTSTATUS status = STATUS_UNSATISFIED_DEPENDENCIES;
    KDPC dpc = { 0 };
    DPC_CONTEXT asyncContext = { 0 };

    auto processorCount = KeQueryActiveProcessorCountEx( ALL_PROCESSOR_GROUPS );

    // Allocate memory to store the devirtualization status of each processor
    asyncContext.status = static_cast< NTSTATUS* >( Inlines::poolAlloc( ( processorCount * sizeof( NTSTATUS ) ) ) );
    if ( !asyncContext.status )
        return STATUS_NO_MEMORY;

    asyncContext.count = 0;
    asyncContext.context = this;
    asyncContext.virtualizeFunction = nullptr;
    asyncContext.deVirtualizeFunction = devirtualizeProcessor;

    KPRINT( "[+]Active Processor Count: %d\n", KeQueryActiveProcessorCountEx( ALL_PROCESSOR_GROUPS ) );

    // Initialize an event for synchronization
    KeInitializeEvent( &asyncContext.event, NotificationEvent, FALSE );

    // Call the DPC routine to devirtualize processors
    KeGenericCallDpc( &DevirtualizeProcessorDPCRoutine, &asyncContext );

    // Wait for all processors to be devirtualized
    KeWaitForSingleObject( &asyncContext.event, Executive, KernelMode, FALSE, nullptr );

    // Check the status of each processor's devirtualization
    for ( auto i = 0; i < processorCount; i++ )
    {
        if ( !NT_SUCCESS( asyncContext.status[ i ] ) )
        {
            status = asyncContext.status[ i ];
        }
    }

    // Free allocated memory for processor status
    if ( asyncContext.status != nullptr )
    {
        Inlines::poolFree( asyncContext.status );
        asyncContext.status = nullptr;
        asyncContext.context = nullptr;
    }

    if ( status == STATUS_UNSATISFIED_DEPENDENCIES )
    {
        status = STATUS_SUCCESS;
    }

    // Free nested tables and processors if necessary
    if ( Hypervisor::nestedTables->msrMap != nullptr )
    {
        Inlines::contigFree( Hypervisor::nestedTables->msrMap, SVM_MSR_PERMISSIONS_MAP_SIZE );
        Hypervisor::nestedTables->msrMap = nullptr;
    }
    if ( Hypervisor::nestedTables != nullptr )
    {
        Inlines::poolFree( Hypervisor::nestedTables );
        Hypervisor::nestedTables = nullptr;
    }

#ifndef NESTED
    if ( Hypervisor::processors != nullptr )
    {
        Inlines::poolFree( Hypervisor::processors );
        Hypervisor::processors = nullptr;
    }
#endif // !NESTED

    if ( !NT_SUCCESS( status ) )
    {
        KPRINT( "[-]Hypervisor Devirtualization Failed\n" );
        return status;
    } else
    {
        KPRINT( "[+]Hypervisor Devirtualized\n" );
        return status;
    }
}
#endif // NESTED

