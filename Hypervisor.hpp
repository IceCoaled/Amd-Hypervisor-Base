#ifndef HYPERVISOR_HPP
#define HYPERVISOR_HPP

#include <intrin.h>
#include "Cpu.hpp"
#include "Svm.hpp"


#define NESTED

#ifdef NESTED
/*
 * @brief Prints debug information if a kernel debugger is present.
 *
 * This macro checks if the kernel debugger is present and, if so, prints
 * the formatted debug information using `DbgPrint`.
 */
#define KPRINT(fmt, ...) \
    if (KD_DEBUGGER_NOT_PRESENT) \
    { \
        NOTHING; \
    } \
    else \
    { \
        DbgPrint(fmt, __VA_ARGS__); \
    } \
    reinterpret_cast<void*>(0)


/*
* @brief Triggers a debug break if a kernel debugger is present.
*
* This macro checks if the kernel debugger is present and, if so, triggers
* a debug break using `__debugbreak`.
*/
#define DEBUG_BREAK() \
    if (KD_DEBUGGER_NOT_PRESENT) \
    { \
        NOTHING; \
    } \
    else \
    { \
        __debugbreak(); \
    } \
    reinterpret_cast<void*>(0)

#endif



/*
* @brief Main class representing the hypervisor functionality.
*
* The `Hypervisor` class encapsulates the logic for initializing, running, and devirtualizing
* a hypervisor across multiple processors.
*/
class Hypervisor
{

private:
	_HV_CPU* processors = nullptr;
	_SHARED_PAGES* nestedTables = nullptr;
	bool ProcessorVirtualized[64] = { false };
	_SETUP_FLAGS setupFlags{};
	_XCR0_REGISTER xCr0BitMask{};


	void CheckForSetupFlags();
	
	void SetupTables(__inout _SHARED_PAGES* sharedPages) const;
	
	NTSTATUS RunProcessors();

	NTSTATUS CheckSupport();
	
	void MsrSetup(__inout void* msrPermsAddr);
	
	bool HyperVirtulizationCheck();
	
	void BuildOutVmcb(__inout _HV_CPU* currentProcessor, __inout const _GUEST_CONTEXT* context);

	bool VirtulizationCheck();
	
	NTSTATUS StartVm();

protected:
	NTSTATUS VirtualizeProcessor();
	
public:
	
	NTSTATUS HypervisorInit()
	{

		if (Hypervisor::CheckSupport() != STATUS_SUCCESS)
		{
			
			KPRINT("[-]HypervisorInit Failed\n");
			return STATUS_UNSATISFIED_DEPENDENCIES;
		}

		Hypervisor::CheckForSetupFlags();

		KPRINT("[+]HypervisorInit Success\n");


		return Hypervisor::StartVm();
	}


#ifdef NESTED
	NTSTATUS Devirtualize(void);

private:
	NTSTATUS DevirtualizeProcessor(void);
#endif

};




EXTERN_C_START

void _sgdt(__out void* descriptor);

__declspec(noreturn) void __stdcall LaunchVm(void* guestVmcbPhysicalAddr);


uint16_t __stdcall ReadCs();
uint16_t __stdcall ReadSs();
uint16_t __stdcall ReadEs();
uint16_t __stdcall ReadDs();
uintmax_t __stdcall ReadEFlags();
uintmax_t __stdcall ReadRsp();
uintmax_t __stdcall ReadRip();


NTKERNELAPI
_IRQL_requires_max_( APC_LEVEL )
_IRQL_requires_min_( PASSIVE_LEVEL ) _IRQL_requires_same_ VOID
KeGenericCallDpc( _In_ PKDEFERRED_ROUTINE Routine, _In_opt_ void* Context );

NTKERNELAPI
_IRQL_requires_( DISPATCH_LEVEL ) _IRQL_requires_same_ VOID
KeSignalCallDpcDone( _In_ void* SystemArgument1 );

NTKERNELAPI
_IRQL_requires_( DISPATCH_LEVEL ) _IRQL_requires_same_ LOGICAL
KeSignalCallDpcSynchronize( _In_ void* SystemArgument2 );

 EXTERN_C_END


#define CAPTURE_GUEST(state)\
	Inlines::memSet(state, 0, sizeof(_GUEST_CONTEXT));\
	_sgdt(&(state)->gdtr);\
	__sidt(&(state)->idtr);\
	(state)->cs = ReadCs();\
	(state)->ss = ReadSs();\
	(state)->ds = ReadDs();\
	(state)->es = ReadEs();\
	(state)->cr0 = __readcr0();\
	(state)->cr2 = __readcr2();\
	(state)->cr3 = __readcr3();\
	(state)->cr4 = __readcr4();\
	(state)->efer = __readmsr(MSR_EFER);\
    (state)->gPat = __readmsr(MSR_PAT);\
	(state)->rflags = ReadEFlags();\
	(state)->rsp = ReadRsp();\
    (state)->rip = ReadRip();


/*
* @brief Type definition for processor function pointers used in DPC context.
*/
using ProcessorFunction = NTSTATUS ( * )( void* context );

/*
* @brief Structure to manage DPC context during processor virtualization.
*/
struct DPC_CONTEXT
{
	NTSTATUS* status;
	KEVENT event;
	long count;
	void* context;
	ProcessorFunction virtualizeFunction;
	ProcessorFunction deVirtualizeFunction;
	KSEMAPHORE dpcSemaphore;
};


#endif // !HYPERVISOR_HXX

