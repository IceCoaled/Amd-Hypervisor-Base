#include "VmExit.hpp"

#pragma intrinsic(__rdtsc)
#pragma intrinsic(__writemsr)
#pragma intrinsic(__readmsr)
#pragma intrinsic(__cpuidex)
#pragma intrinsic(_xsetbv)
#pragma intrinsic(__rdtscp)

namespace Vmexits
{
	// Determines if the current privilege level (CPL) is 3 and whether nRip is within the expected range.
	bool Cpl3( __inout _HV_CPU* currentProcessor )
	{
		return currentProcessor->guestVmcb.stateSaveArea.cpl == DPL_USER && currentProcessor->guestVmcb.controlArea.nRip < 0x7FFFFFFEFFFF;
	}

	// Injects a general protection fault exception into the guest.
	void GeneralException( __inout _HV_CPU* currentProcessor )
	{
		_EVENTINJ exception = { 0 };
		exception.bit.type = INTERRUPT_TYPE_HARDWARE_EXCEPTION;
		exception.bit.valid = 1;
		exception.bit.errorCodeValid = 1;
		exception.bit.vector = EXCEPTION_VECTOR_GENERAL_PROTECTION_FAULT;

		currentProcessor->guestVmcb.controlArea.eventInj = exception.value;
	}

	// Injects an undefined opcode exception into the guest.
	void InvalidException( __inout _HV_CPU* currentProcessor )
	{
		_EVENTINJ exception = { 0 };
		exception.bit.type = INTERRUPT_TYPE_HARDWARE_EXCEPTION;
		exception.bit.valid = 1;
		exception.bit.errorCodeValid = 1;
		exception.bit.vector = EXCEPTION_VECTOR_UNDEFINED_OPCODE;

		currentProcessor->guestVmcb.controlArea.eventInj = exception.value;
	}

	// Handles CPUID instruction VM exits.
	void CpuidExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context )
	{
		_SEGMENT_ATTRIBUTES attribute = { 0 };
		auto currentLeaf = static_cast< int32_t >( context->guestRegisters->rax.GetR64() );
		auto subLeaf = static_cast< int32_t >( context->guestRegisters->rcx.GetR64() );

		int32_t registers[ 4 ] = { 0 };
		__cpuidex( registers, currentLeaf, subLeaf );

	#ifdef NESTED
			// Custom handling for certain CPUID leaves in nested mode
		switch ( currentLeaf )
		{
			case CPUID_PROCESSOR_AND_PROCESSOR_FEATURE_IDENTIFIERS:
				// Set hypervisor present bit
				registers[ 2 ] |= CPUID_FN0000_0001_ECX_HYPERVISOR_PRESENT;
				break;
			case CPUID_HV_VENDOR_AND_MAX_FUNCTIONS:
				// Set vendor-specific values
				registers[ 0 ] = CPUID_HV_MAX;
				registers[ 1 ] = 'CecI';
				registers[ 2 ] = 'elao';
				registers[ 3 ] = '   d';
				break;
			case CPUID_HV_INTERFACE:
				// Set hypervisor interface signature
				registers[ 0 ] = '0#vH';
				registers[ 1 ] = 0;
				registers[ 2 ] = 0;
				registers[ 3 ] = 0;
				break;
			case CPUID_UNLOAD:
				// Handle CPUID_UNLOAD with specific privilege check
				if ( subLeaf == CPUID_UNLOAD )
				{
					attribute.value = currentProcessor->guestVmcb.stateSaveArea.ss.attributes.value;
					if ( attribute.bit.dpl == DPL_SYSTEM )
					{
						context->exit = true;
					}
				}
				break;
			default:
				break;
		}
	#endif

			// Update guest registers with the result of CPUID
		context->guestRegisters->rax = registers[ 0 ];
		context->guestRegisters->rbx = registers[ 1 ];
		context->guestRegisters->rcx = registers[ 2 ];
		context->guestRegisters->rdx = registers[ 3 ];

		// Move RIP to the next instruction
		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}

	// Handles MSR read/write VM exits.
	void MsrExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context )
	{
		auto msrRegister = static_cast< uint32_t >( context->guestRegisters->rcx & MAXUINT32 );
		auto writeAccess = currentProcessor->guestVmcb.controlArea.exitInfo1 != 0;
		ULARGE_INTEGER value = { 0 };

		if ( OutsideMsr( msrRegister ) )
		{
			// Trigger general exception for unsupported MSRs
			Vmexits::GeneralException( currentProcessor );
			goto end;
		} else if ( writeAccess )
		{
			// Handle MSR write
			value.LowPart = static_cast< ULONG >( context->guestRegisters->rax & MAXUINT32 );
			value.HighPart = static_cast< ULONG >( context->guestRegisters->rdx & MAXUINT32 );

			if ( msrRegister == MSR_EFER )
			{
				_EFER_REGISTER efer{};
				efer = value.QuadPart;
				if ( !currentProcessor->guestVmcb.stateSaveArea.efer.cmpSvmeBit( efer ) )
				{
					Vmexits::InvalidException( currentProcessor );
				} else if ( !currentProcessor->guestVmcb.stateSaveArea.efer.cmpLmaBit( efer ) ||
						   !currentProcessor->guestVmcb.stateSaveArea.efer.cmpLmeBit( efer ) ||
						   !currentProcessor->guestVmcb.stateSaveArea.efer.cmpNxeBit( efer ) )
				{
					Vmexits::InvalidException( currentProcessor );
				}

				currentProcessor->guestVmcb.stateSaveArea.efer = efer;
			}

			// Write the MSR
			__writemsr( msrRegister, value.QuadPart );
		} else
		{
			// Handle MSR read
			value.QuadPart = __readmsr( msrRegister );
			context->guestRegisters->rax = value.LowPart;
			context->guestRegisters->rdx = value.LowPart;
		}

	end:
		// Move RIP to the next instruction
		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}

	// Checks if the MSR is within allowed ranges.
	bool OutsideMsr( __inout const uint32_t& msr )
	{
		if ( ( ( msr > 0 ) && ( msr < 0x00001FFF ) ) || ( ( msr > 0xC0000000 ) && ( msr < 0xC0001FFF ) ) || ( msr > 0xC0010000 ) && ( msr < 0xC0011FFF ) )
			return true;

		return false;
	}

	// Handles RDTSC instruction VM exits.
	void RdtscExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context )
	{
		auto time = __rdtsc();
		context->guestRegisters->rax = static_cast< uint32_t >( time );
		context->guestRegisters->rdx = static_cast< uint32_t >( time >> 32 );

		// Move RIP to the next instruction
		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}

	// Handles XSETBV instruction VM exits.
	void XSetBvExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context )
	{
		_XCR0_REGISTER xcr0{};
		auto& cachedXcr0 = currentProcessor->hostStack.layout.xSetBvMask;
		auto& cr4 = currentProcessor->guestVmcb.stateSaveArea.cr4;

		// Check if OSXSAVE is enabled; if it is, inject an invalid exception
		if ( cr4.bit.osxsave == 1 )
		{
			InvalidException( currentProcessor );
			goto end;
		}

		// XSETBV must be called with RCX == 0
		if ( context->guestRegisters->rcx != 0 )
		{
			GeneralException( currentProcessor );
			goto end;
		}

		xcr0 = ( ( context->guestRegisters->rdx & MAXUINT32 ) | ( context->guestRegisters->rax & MAXUINT32 ) );

		// Ensure that XCR0 matches the cached value; otherwise, trigger exception
		if ( xcr0 != cachedXcr0 )
		{
			GeneralException( currentProcessor );
			goto end;
		}

		// Verify X87 state is enabled
		if ( !xcr0.CheckX87() )
		{
			GeneralException( currentProcessor );
			goto end;
		}

		// Execute XSETBV
		_xsetbv( static_cast< uint32_t >( context->guestRegisters->rcx.GetR64() ), xcr0.GetValue() );

	end:
		// Move RIP to the next instruction
		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}

	// Handles RDTSCP instruction VM exits.
	void RdtscpExit( __inout _HV_CPU* currentProcessor, __inout _EXIT_CONTEXT* context )
	{
		uint32_t rdtscpinput = 0;
		auto time = __rdtscp( &rdtscpinput );

		context->guestRegisters->rax = static_cast< uint32_t >( time );
		context->guestRegisters->rdx = static_cast< uint32_t >( time >> 32 );
		context->guestRegisters->rcx = static_cast< uint32_t >( rdtscpinput );

		// Move RIP to the next instruction
		currentProcessor->guestVmcb.stateSaveArea.rip = currentProcessor->guestVmcb.controlArea.nRip;
	}
};
