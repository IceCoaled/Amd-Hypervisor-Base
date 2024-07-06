#ifndef HYPERVISOR_HPP
#define HYPERVISOR_HPP

#include "Cpu.hpp"
#include "Svm.hpp"
#include "Eprocess.hpp"
#include "KThread.hpp"


#define KPRINT(fmt, ...) DbgPrint(fmt, __VA_ARGS__);


class Hypervisor
{

private:
	_SHARED_PAGES* nestedTables;
	_SHARED_PAGES* hostPages;
	void* msrMap;
	bool ProcessorVirtualized[64] = { false };

private:
	_SETUP_FLAGS setupFlags;
	_CR3_REGISTER systemCr3;
	__EPROCESS* systemProcess;
	NTSTATUS initCheck;
	_EFER_REGISTER eferBitMask;
	_XCR0_REGISTER xCr0BitMask;
private:
	
	NTSTATUS SetupHostDetals(__inout _HV_CPU* currentProcessor);

	void CheckForSetupFlags();
	
	void SetupTables(__inout _SHARED_PAGES* sharedPages, __in const bool Flag) const;
	
	NTSTATUS RunProcessors();

	NTSTATUS CheckSupport();
	
	void MsrSetup(__in void* msrPermsAddr);
	
	bool VirtulizationCheck(__in const uint32_t& processorIndex);
	
	void BuildOutVmcb(__inout _HV_CPU* currentProcessor);
	
	NTSTATUS VirtualizeProcessor(__in const uint32_t processorIndex);
	
	void StartVm();
	
public:
	
	void HypervisorInit()
	{
		///class initialization
		this->setupFlags = { 0 },
			systemCr3 = { 0 },
			eferBitMask,
			xCr0BitMask,
			nestedTables = nullptr,
			hostPages = nullptr,
			msrMap = nullptr,
			systemProcess = nullptr,
			initCheck = -1;
		
		
		this->initCheck = this->CheckSupport();
		if (this->initCheck != STATUS_SUCCESS)
		{
			
			KPRINT("[-]HypervisorInit Failed\n");
			return;
		}

		KPRINT("[+]HypervisorInit Success\n");	

		this->CheckForSetupFlags();

		this->StartVm();
	}
	
	~Hypervisor() = default;	
};




EXTERN_C_START

void _sgdt(__out void* descriptor);
void __stdcall VmEnter(void* guestVmcbPhysicalAddr);

NTSTATUS
NTSYSAPI
NTAPI
ZwYieldExecution(
	VOID
);


uint16_t __stdcall ReadCs();
uint16_t __stdcall ReadSs();
uint16_t __stdcall ReadEs();
uint16_t __stdcall ReadDs();
 
uintmax_t __stdcall ReadRsp();
uintmax_t __stdcall ReadRip();

 EXTERN_C_END


#endif // !HYPERVISOR_HXX

