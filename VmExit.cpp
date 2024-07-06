#include "VmExit.hpp"


#pragma intrinsic(__rdtsc)
#pragma intrinsic(__writemsr)
#pragma intrinsic(__readmsr)
#pragma intrinsic(__cpuidex)
#pragma intrinsic(_xsetbv)
#pragma intrinsic(__rdtscp)



namespace Vmexits
{
	
	
	void FlushTlbEntries(__in _HV_CPU* currentProcessor, __in uint32_t flushType)
	{
		currentProcessor->guestVmcb.controlArea.guestAsidTlbControl.tlb_control |= flushType;
	}



	void CleanVmcb(__in _HV_CPU* currentProcessor, __in bool clean)
	{
		if (clean) currentProcessor->guestVmcb.controlArea.vmcbClean.value = 0x0U;
		else currentProcessor->guestVmcb.controlArea.vmcbClean.value = 0xFFFFFFFFU;
	}

	
	bool Cpl3(__in _HV_CPU* currentProcessor)
	{
		return currentProcessor->guestVmcb.stateSaveArea.cpl == DPL_USER && currentProcessor->guestVmcb.controlArea.nRip < 0x7FFFFFFEFFFF;
	}



	void GeneralException(__in _HV_CPU* currentProcessor)
	{
		_EVENTINJ exception = { 0 };

		exception.bit.type = INTERRUPT_TYPE_HARDWARE_EXCEPTION;
		exception.bit.valid = 1;
		exception.bit.errorCodeValid = 1;
		exception.bit.vector = EXCEPTION_VECTOR_GENERAL_PROTECTION_FAULT;

		currentProcessor->guestVmcb.controlArea.eventInj = exception.value;
	}



	void InvalidException(__in _HV_CPU* currentProcessor)
	{
		_EVENTINJ exception = { 0 };

		exception.bit.type = INTERRUPT_TYPE_HARDWARE_EXCEPTION;
		exception.bit.valid = 1;
		exception.bit.errorCodeValid = 1;
		exception.bit.vector = EXCEPTION_VECTOR_UNDEFINED_OPCODE;

		currentProcessor->guestVmcb.controlArea.eventInj = exception.value;
	}



	void CpuidExit(__in _HV_CPU* currentProcessor)
	{
		auto* context = &currentProcessor->hostStack.layout.guestRegisters;
		
		_SEGMENT_ATTRIBUTES attributes = { 0 };
		auto currentLeaf = static_cast<uint32_t>(context->rax.GetR64());
		auto subLeaf = static_cast<uint32_t>(context->rcx.GetR64());

		int32_t registers[4] = { 0 };
		__cpuidex(registers, currentLeaf, subLeaf);


		context->rax = registers[0];
		context->rbx = registers[1];
		context->rcx = registers[2];
		context->rdx = registers[3];


		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}



	void MsrExit(__in _HV_CPU* currentProcessor)
	{
		auto* context = &currentProcessor->hostStack.layout.guestRegisters;
		auto msrRegister = static_cast<uint32_t>(context->rcx & MAXUINT32);
		auto writeAccess = currentProcessor->guestVmcb.controlArea.exitInfo1 != 0;
		ULARGE_INTEGER value = { 0 };

#ifdef HYPER_V
		if (!HypervMsr(msrRegister))
		{
			Vmexits::GeneralException(currentProcessor);
		}
		else if (writeAccess)
		{
			value.LowPart = context->rax & MAXUINT32;
			value.HighPart = context->rdx & MAXUINT32;

			if (msrRegister == MSR_EFER)
			{
				_EFER_REGISTER efer{};
				efer = value.QuadPart;
				if (!currentProcessor->guestVmcb.stateSaveArea.efer.cmpSvmeBit(efer))
				{
					Vmexits::InvalidException(currentProcessor);
					goto end;
				}

				if (!currentProcessor->guestVmcb.stateSaveArea.efer.cmpLmaBit(efer) ||
					!currentProcessor->guestVmcb.stateSaveArea.efer.cmpLmeBit(efer) ||
					!currentProcessor->guestVmcb.stateSaveArea.efer.cmpNxeBit(efer))
				{
					Vmexits::FlushTlbEntries(currentProcessor, TLB_FLUSH_GLOBAL_GUEST_TLB);
				}
			}

			__writemsr(msrRegister, value.QuadPart);
		}
		else
		{
			value.QuadPart = __readmsr(msrRegister);

			value.LowPart = context->rax & MAXUINT32;
			value.HighPart = context->rdx & MAXUINT32;
		}
#else
		
		if (OutSideMsrMap(msrRegister))
		{
			Vmexits::GeneralException(currentProcessor);
		}
		else if (writeAccess)
		{
			value.LowPart = context->rax & MAXUINT32;
			value.HighPart = context->rdx & MAXUINT32;
			
			
			if (msrRegister == MSR_EFER)
			{
				_EFER_REGISTER efer{};
				efer = value.QuadPart;
				if (!currentProcessor->guestVmcb.stateSaveArea.efer.cmpSvmeBit(efer))
				{
					Vmexits::InvalidException(currentProcessor);
					goto end;
				}

				if (!currentProcessor->guestVmcb.stateSaveArea.efer.cmpLmaBit(efer) ||
					!currentProcessor->guestVmcb.stateSaveArea.efer.cmpLmeBit(efer) ||
					!currentProcessor->guestVmcb.stateSaveArea.efer.cmpNxeBit(efer))
				{
					Vmexits::FlushTlbEntries(currentProcessor, TLB_FLUSH_GLOBAL_GUEST_TLB);
				}
			}

			__writemsr(msrRegister, value.QuadPart);
		}
		else
		{
			value.QuadPart = __readmsr(msrRegister);

			value.LowPart = context->rax & MAXUINT32;
			value.HighPart = context->rdx & MAXUINT32;
		}
#endif //!HYPER_V 

		end:

		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}



	bool OutSideMsrMap(__in const uint32_t& msr)
	{
		if ((msr > 0) && (msr < 0xc0011FFF)) return true;

		return false;
		
	}



	bool HypervMsr(__in const uint32_t& msr)
	{
		if (((msr > 0) && (msr < 0x00001FFF)) || ((msr > 0xC0000000) && (msr < 0xC0001FFF)) || (msr > 0xC0010000) && (msr < 0xC0011FFF)) return true;

		return false;
	}



	void RdtscExit(__in _HV_CPU* currentProcessor)
	{
		auto* context = &currentProcessor->hostStack.layout.guestRegisters;
		auto time = __rdtsc();
		context->rax = (time & MAXUINT32);
		context->rdx = (time >> 32);

		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}



	void XSetBvExit(__in _HV_CPU* currentProcessor)
	{
		auto* context = &currentProcessor->hostStack.layout.guestRegisters;
		_XCR0_REGISTER xcr0{};
		auto& cachedXcr0 = currentProcessor->hostStack.layout.xSetBvMask; 
		auto& cr4 = currentProcessor->guestVmcb.stateSaveArea.cr4;


		if (cr4.bit.osxsave == 1)
		{
			InvalidException(currentProcessor);
			goto end;
		}
			
		if (context->rcx != 0)
		{
			GeneralException(currentProcessor);
			goto end;
		}
			

		xcr0 = ((context->rdx & MAXUINT32) | (context->rax & MAXUINT32));

		if (xcr0 != cachedXcr0)
		{
			GeneralException(currentProcessor);
			goto end;
		}
			
		if (!xcr0.CheckX87())
		{
			GeneralException(currentProcessor);
			goto end;
		}
			
		_xsetbv((context->rcx.GetR64()), xcr0.GetValue());

		end:

		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}


	void RdtscpExit(__in _HV_CPU* currentProcessor)
	{
		auto* context = &currentProcessor->hostStack.layout.guestRegisters;
		uint32_t rdtscpinput = 0;
		auto time = __rdtscp(&rdtscpinput);

		context->rax = (time & MAXUINT32);
		context->rdx = (time >> 32);
		context->rcx = rdtscpinput;

		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}


};