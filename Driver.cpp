#include "Hypervisor.hpp"
#include "CustomFunctions.hpp"

#define POOL_NX_OPTIN 1
EXTERN_C DRIVER_INITIALIZE DriverEntry;



#ifdef NESTED
EXTERN_C DRIVER_UNLOAD UnloadDriver;


/**
 * @brief Unloads the hypervisor driver and devirtualizes all processors.
 *
 * @param pDriverObject Pointer to the driver object.
 *
 * This function is called when the driver is being unloaded. It will attempt to devirtualize all
 * processors that were virtualized by the hypervisor and then free allocated resources. If devirtualization
 * fails, an error message is logged.
 */
_Use_decl_annotations_
void UnloadDriver( PDRIVER_OBJECT pDriverObject )
{
    // Cast the driver start to a Hypervisor object and initiate devirtualization.
    auto* deVirtualize = static_cast< Hypervisor* >( pDriverObject->DriverStart );

    // Attempt to devirtualize all processors.
    auto status = deVirtualize->Devirtualize();

    // Log the status of devirtualization.
    if ( !NT_SUCCESS( status ) )
    {
        KPRINT( "[-]Failed to devirtualize processors\n" );
    } else
    {
        KPRINT( "[+]Devirtualized Processors\n" );
    }

    // Free allocated memory for the module list and hypervisor object.
    Inlines::poolFree( &Inlines::moduleList );
    Inlines::contigFree( pDriverObject->DriverStart, sizeof( Hypervisor ) );
    KPRINT( "[+]Driver Unloaded\n" );
}

#endif // NESTED



/**
 * @brief Entry point for the hypervisor driver.
 *
 * @param DriverObject Pointer to the driver object created by the system.
 * @param RegistryPath Pointer to the registry path for the driver.
 * @return NTSTATUS Status code indicating success or failure of driver initialization.
 *
 * This function initializes the hypervisor by setting up required resources, including the module list,
 * kernel base, and PTE address. If any step fails, it logs an error and exits. Upon successful initialization,
 * it sets the UnloadDriver function as the driver unload routine (if running on Hyper-V).
 */
_Use_decl_annotations_
EXTERN_C
NTSTATUS DriverEntry( __in PDRIVER_OBJECT DriverObject, __in PUNICODE_STRING RegistryPath )
{
    UNREFERENCED_PARAMETER( RegistryPath );

    KPRINT( "[+]DriverEntry Called\n" );

#ifdef NESTED
    // Set the unload function to handle devirtualization and cleanup.
    DriverObject->DriverUnload = UnloadDriver;
#endif // NESTED

    // Initialize driver runtime with NX pool opt-in for security.
    ExInitializeDriverRuntime( DrvRtPoolNxOptIn );

    // Retrieve the list of loaded modules in the system.
    Inlines::GetModuleList( &Inlines::moduleList );
    if ( Inlines::moduleList->count == 0 )
    {
        KPRINT( "[-]Failed To Get Module List\n" );
        Inlines::poolFree( &Inlines::moduleList );
        return STATUS_UNSUCCESSFUL;
    }

    KPRINT( "[+]Module List Success\n" );

    // Find the kernel base address.
    Inlines::kernelBase = Inlines::FindModule( &Inlines::moduleList, &Inlines::szKernel, "ntoskrnl.exe" );
    if ( !Inlines::kernelBase )
    {
        KPRINT( "[-]Failed To Find Kernel Base\n" );
        Inlines::poolFree( &Inlines::moduleList );
        return STATUS_UNSUCCESSFUL;
    }

    KPRINT( "[+]Found kernel Base Located At : 0x%I64X\n", Inlines::kernelBase );

    // Scan for the MiGetPteAddress function within the kernel.
    Inlines::miGetPteAddress = Inlines::signatureScanner(
        ( UCHAR* )Inlines::kernelBase,
        Inlines::szKernel,
        Inlines::_MIGETPTEADDRESS,
        sizeof( Inlines::_MIGETPTEADDRESS )
    );

    if ( !Inlines::miGetPteAddress )
    {
        KPRINT( "[-]Failed To Find MiGetPteAddress\n" );
        Inlines::poolFree( &Inlines::moduleList );
        return STATUS_UNSUCCESSFUL;
    }

    KPRINT( "[+]Found MiGetPteAddress Located At : 0x%I64X\n", Inlines::miGetPteAddress );

    // Allocate memory for the hypervisor object.
    auto hypervisor = static_cast< Hypervisor* >( Inlines::contigAlloc( sizeof( Hypervisor ) ) );
    if ( !hypervisor )
    {
        KPRINT( "[-]Failed to allocate hypervisor\n" );
        Inlines::poolFree( &Inlines::moduleList );
        return STATUS_UNSUCCESSFUL;
    }

    // Store the hypervisor object in the driver start for future reference.
    DriverObject->DriverStart = static_cast< void* >( hypervisor );

    // Initialize the hypervisor, logging and handling failures.
    if ( !NT_SUCCESS( hypervisor->HypervisorInit() ) )
    {
        KPRINT( "[-]Failed to initialize hypervisor\n" );
        Inlines::poolFree( &Inlines::moduleList );
        Inlines::contigFree( hypervisor, sizeof( Hypervisor ) );
        return STATUS_UNSUCCESSFUL;
    }

    return STATUS_SUCCESS;
}


