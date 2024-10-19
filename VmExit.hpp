#ifndef VMEXIT_HPP
#define VMEXIT_HPP

#include "hypervisor.hpp"

namespace Vmexits
{
	void CpuidExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context );
	void MsrExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context );
	bool OutsideMsr( __inout const uint32_t& msr );
	void RdtscExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context );
	void XSetBvExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context );
	void RdtscpExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context );
	void InvalidException( __inout _HV_CPU* currentProcessor );
	void GeneralException( __inout _HV_CPU* currentProcessor );
	bool Cpl3( __inout _HV_CPU* currentProcessor );

	EXTERN_C bool __stdcall HandleVmExit( __inout _HV_CPU* currentProcessor, __inout _GUEST_REGISTERS* guestRegisters )
	{
		_EXIT_CONTEXT context = { 0 };
		context.guestRegisters = guestRegisters;
		context.exit = false;

		// Load the host VMCB (Virtual Machine Control Block)
		__svm_vmload( currentProcessor->hostStack.layout.hostVmcbPa );

		// Update guest register RAX with saved value
		guestRegisters->rax = currentProcessor->guestVmcb.stateSaveArea.rax;

		// Verify that the reserved layout matches expected value (debug check)
		NT_VERIFY( currentProcessor->hostStack.layout.reserved0 == MAXINT64 );

		// Handle VM exit based on the exit code in the guest VMCB control area
		switch ( currentProcessor->guestVmcb.controlArea.exitCode )
		{
			case VMEXIT_VMRUN:
			case VMEXIT_VMLOAD:
			case VMEXIT_VMSAVE:
			case VMEXIT_CLGI:
			case VMEXIT_SKINIT:
			case VMEXIT_STGI:
				// For unsupported VM exits, handle as an invalid exception
				Vmexits::InvalidException( currentProcessor );
				break;
			case VMEXIT_CPUID:
				// Handle CPUID instruction exit
				CpuidExit( currentProcessor, &context );
				break;
			case VMEXIT_RDTSC:
				// Handle RDTSC instruction exit
				RdtscExit( currentProcessor, &context );
				break;
			case VMEXIT_MSR:
				// Handle MSR read/write exit
				MsrExit( currentProcessor, &context );
				break;
			case VMEXIT_RDTSCP:
				// Handle RDTSCP instruction exit
				RdtscpExit( currentProcessor, &context );
				break;
			case VMEXIT_XSETBV:
				// Handle XSETBV instruction exit
				XSetBvExit( currentProcessor, &context );
				break;
			case VMEXIT_VMMCALL:
				// Placeholder for VMMCALL exit, not implemented
				// VmmcallExit(currentProcessor, &context);
			#ifdef NESTED
				// In nested virtualization, update the RIP to the next instruction
				currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
			#endif // NESTED
				break;
			default:
				// For unknown exit codes, trigger a bug check with relevant information
				DEBUG_BREAK();
				KeBugCheckEx( 0xFA11UL, Vmexits::Cpl3( currentProcessor ), currentProcessor->guestVmcb.stateSaveArea.rip, currentProcessor->guestVmcb.controlArea.exitCode, 0 );
				break;
		}

		// If the context indicates a VM exit, handle the exit conditions
		if ( context.exit )
		{
			// Set guest register values to reflect the exit state
			context.guestRegisters->rax = reinterpret_cast< uintmax_t >( currentProcessor ) & MAXUINT32;
			context.guestRegisters->rbx = currentProcessor->guestVmcb.controlArea.nRip;
			context.guestRegisters->rcx = currentProcessor->guestVmcb.stateSaveArea.rsp;
			context.guestRegisters->rdx = reinterpret_cast< uintmax_t >( currentProcessor ) >> 32;

			// Load the guest VMCB for further processing
			__svm_vmload( currentProcessor->hostStack.layout.guestVmcbPa );

			// Disable interrupts and set the Global Interrupt Flag (GIF)
			_disable();
			__svm_stgi();

			// Update EFER register, disabling the SVM mode, and restore RFLAGS
			__writemsr( MSR_EFER, __readmsr( MSR_EFER ) & ~EFER_SVME );
			__writeeflags( currentProcessor->guestVmcb.stateSaveArea.rflags.value );
			goto exit;
		}

		// Restore RAX register from the updated context
		currentProcessor->guestVmcb.stateSaveArea.rax = context.guestRegisters->rax.GetR64();

	exit:
		// Verify reserved layout to ensure memory integrity
		NT_VERIFY( currentProcessor->hostStack.layout.reserved0 == MAXINT64 );

		// Return whether the context has requested a VM exit
		return context.exit;
	}

}; // namespace Vmexits

#endif // !VMEXIT_HXX
