#ifndef VMEXIT_HPP
#define VMEXIT_HPP

#define HYPER_V

#include "hypervisor.hpp"

#include <intrin.h>


namespace Vmexits
{
	void CpuidExit(__in _HV_CPU* currentProcessor);

	void MsrExit(__in _HV_CPU* currentProcessor);

	bool HypervMsr(__in const uint32_t& msr);

	bool OutSideMsrMap(__in const uint32_t& msr);

	void RdtscExit(__in _HV_CPU* currentProcessor);

	void XSetBvExit(__in _HV_CPU* currentProcessor);

	void RdtscpExit(__in _HV_CPU* currentProcessor);

	void InvalidException(__in _HV_CPU* currentProcessor);

	void GeneralException(__in _HV_CPU* currentProcessor);

	bool Cpl3(__in _HV_CPU* currentProcessor);

	void FlushTlbEntries(__in _HV_CPU* currentProcessor, __in uint32_t flushType);

	void CleanVmcb(__in _HV_CPU* currentProcessor, __in bool clean);



	EXTERN_C void __stdcall HandleVmExit(__in _HV_CPU* currentProcessor)
	{
		
		currentProcessor->hostStack.layout.guestRegisters.rax = currentProcessor->guestVmcb.stateSaveArea.rax;
		
		switch (currentProcessor->guestVmcb.controlArea.exitCode)
		{
		case VMEXIT_VMRUN:
		case VMEXIT_VMLOAD:
		case VMEXIT_VMSAVE:
		case VMEXIT_CLGI:	
		case VMEXIT_SKINIT:		
		case VMEXIT_STGI:
			Vmexits::InvalidException(currentProcessor);
			break;
		case VMEXIT_CPUID:
			CpuidExit(currentProcessor);
			break;
		case VMEXIT_RDTSC:
			RdtscExit(currentProcessor);
			break;
		case VMEXIT_MSR:
			MsrExit(currentProcessor);
			break;
		case VMEXIT_RDTSCP:
			RdtscpExit(currentProcessor);
			break;
		case VMEXIT_XSETBV:
			XSetBvExit(currentProcessor);
			break;
		default:
			KeBugCheckEx(0xFA11UL, Vmexits::Cpl3(currentProcessor), currentProcessor->guestVmcb.stateSaveArea.rip, currentProcessor->guestVmcb.controlArea.exitCode, 0);
			break;
		}



		currentProcessor->guestVmcb.stateSaveArea.rax = currentProcessor->hostStack.layout.guestRegisters.rax.GetR64();
	}


}; // namespace Vmexits

#endif // !VMEXIT_HXX


