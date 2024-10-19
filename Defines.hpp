
#ifndef DEFINES_HPP
#define DEFINES_HPP

// Memory and Page Table Definitions
#define PAGE_TABLE_SIZE 0x200      // Size of page table (typically 512 entries)
#define PAGE_SIZE 0x1000           // Standard memory page size (4KB)
#define KERNEL_STACK_SIZE 0x6000   // Size of kernel stack (24KB)
#define CPUID_UNLOAD 0xC0A1ED      // Unique identifier used to signal CPUID unload

// Index Masks and Page Shifts for Address Translation
#define INDEX_MASK 0x1FF           // Mask used for indexing into page tables (9 bits)
#define PD_PAGE_SHIFT 21           // Bit shift for page directory
#define PML4_PAGE_SHIFT 39         // Bit shift for Page Map Level 4 (PML4)
#define PDP_PAGE_SHIFT 30          // Bit shift for Page Directory Pointer (PDP)
#define TABLE_PAGE_SIZE 0x200      // Size of page table entries (typically 512)

// MSR (Model Specific Registers) Definitions
#define MSR_GHCB 0xC0010130        // MSR for GHCB (Guest Hypervisor Communication Block)
#define MSR_TOM 0xC0010135         // MSR for Top of Memory
#define MSR_DBG_CTL 0x000001D9     // MSR for Debug Control
#define MSR_APIC_BASE 0x0000001b   // MSR for APIC Base Address
#define MSR_PAT 0x00000277         // MSR for Page Attribute Table
#define MSR_EFER 0xc0000080        // MSR for Extended Feature Enable Register (EFER)
#define EFER_SVME (1UL << 12)      // Enable SVM in EFER register

// Descriptor Privilege Levels (DPL)
#define DPL_SYSTEM 0               // System level (Ring 0)
#define DPL_USER 3                 // User level (Ring 3)

// CPUID Feature Masks for Various Capabilities
#define CPUID_FN8000_0001_ECX_SVM (1UL << 2)                     // SVM feature in CPUID
#define CPUID_FN0000_0001_ECX_HYPERVISOR_PRESENT (1UL << 31)     // Indicates hypervisor presence
#define CPUID_FN8000_000A_EDX_NP (1UL << 0)                      // Nested Paging feature
#define CPUID_FN8000_0001_EDX_LONG_MODE (1UL << 29)              // Long Mode (x86-64) feature
#define CPUID_FN0000_0001_ECX_X2APIC_MODE (1UL << 21)            // x2APIC mode feature
#define CPUID_FN0000_0001_EDX_APIC_MODE (1UL << 9)               // APIC mode feature

// CPUID Identifiers for Various Functions
#define CPUID_MAX_STANDARD_FN_NUMBER_AND_VENDOR_STRING 0x00000000       // CPUID for vendor and max standard function number
#define CPUID_PROCESSOR_AND_PROCESSOR_FEATURE_IDENTIFIERS 0x00000001    // CPUID for processor feature identifiers
#define CPUID_PROCESSOR_AND_PROCESSOR_FEATURE_IDENTIFIERS_EX 0x80000001 // CPUID for extended processor feature identifiers
#define CPUID_SVM_FEATURES 0x8000000a                                    // CPUID for SVM features
#define CPUID_SVM_FEATURES_EX 0x80000001                                 // CPUID for extended SVM features

// MSRs for SVM (Secure Virtual Machine) Management
#define SVM_MSR_VM_CR 0xc0010114                   // MSR for VM Control Register
#define SVM_MSR_VM_HSAVE_PA 0xc0010117             // MSR for VM Host Save Area
#define SVM_VM_CR_SVMDIS (1UL << 4)                // Disable SVM bit in VM_CR
#define SVM_MSR_PERMISSIONS_MAP_SIZE (0x2000)      // Size of MSR permissions map

// Flags for Nested Paging
#define SVM_NP_ENABLE (1UL << 0)                   // Enable Nested Paging

// Hypervisor CPUID Definitions
#define CPUID_HV_VENDOR_AND_MAX_FUNCTIONS 0x40000000  // CPUID function for hypervisor vendor and max functions
#define CPUID_HV_INTERFACE 0x40000001                 // CPUID function for hypervisor interface
#define CPUID_HV_MAX CPUID_HV_INTERFACE               // Max CPUID function for hypervisor features

// Definitions for I/O Interception Fields in SVM
#define SVM_IOIO_STR_SHIFT 2                   // Shift for I/O string field
#define SVM_IOIO_REP_SHIFT 3                   // Shift for I/O REP field
#define SVM_IOIO_SIZE_SHIFT 4                  // Shift for I/O size field
#define SVM_IOIO_ASIZE_SHIFT 7                 // Shift for I/O address size field
#define SVM_IOIO_TYPE_MASK 1                   // Mask for I/O type
#define SVM_IOIO_STR_MASK (1 << SVM_IOIO_STR_SHIFT)  // Mask for I/O string field
#define SVM_IOIO_REP_MASK (1 << SVM_IOIO_REP_SHIFT)  // Mask for I/O REP field
#define SVM_IOIO_SIZE_MASK (7 << SVM_IOIO_SIZE_SHIFT) // Mask for I/O size field
#define SVM_IOIO_ASIZE_MASK (7 << SVM_IOIO_ASIZE_SHIFT) // Mask for I/O address size field


// VM exit codes for reading from control registers (CR0-CR15)
#define VMEXIT_CR0_READ             0x0000  // VM exit due to read access to CR0
#define VMEXIT_CR1_READ             0x0001  // VM exit due to read access to CR1 (reserved)
#define VMEXIT_CR2_READ             0x0002  // VM exit due to read access to CR2 (page fault linear address)
#define VMEXIT_CR3_READ             0x0003  // VM exit due to read access to CR3 (page directory base register)
#define VMEXIT_CR4_READ             0x0004  // VM exit due to read access to CR4
#define VMEXIT_CR5_READ             0x0005  // VM exit due to read access to CR5 (reserved)
#define VMEXIT_CR6_READ             0x0006  // VM exit due to read access to CR6 (reserved)
#define VMEXIT_CR7_READ             0x0007  // VM exit due to read access to CR7 (reserved)
#define VMEXIT_CR8_READ             0x0008  // VM exit due to read access to CR8 (task priority register, TPR)
#define VMEXIT_CR9_READ             0x0009  // VM exit due to read access to CR9 (reserved)
#define VMEXIT_CR10_READ            0x000a  // VM exit due to read access to CR10 (reserved)
#define VMEXIT_CR11_READ            0x000b  // VM exit due to read access to CR11 (reserved)
#define VMEXIT_CR12_READ            0x000c  // VM exit due to read access to CR12 (reserved)
#define VMEXIT_CR13_READ            0x000d  // VM exit due to read access to CR13 (reserved)
#define VMEXIT_CR14_READ            0x000e  // VM exit due to read access to CR14 (reserved)
#define VMEXIT_CR15_READ            0x000f  // VM exit due to read access to CR15 (reserved)

// VM exit codes for writing to control registers (CR0-CR15)
#define VMEXIT_CR0_WRITE            0x0010  // VM exit due to write access to CR0
#define VMEXIT_CR1_WRITE            0x0011  // VM exit due to write access to CR1 (reserved)
#define VMEXIT_CR2_WRITE            0x0012  // VM exit due to write access to CR2
#define VMEXIT_CR3_WRITE            0x0013  // VM exit due to write access to CR3
#define VMEXIT_CR4_WRITE            0x0014  // VM exit due to write access to CR4
#define VMEXIT_CR5_WRITE            0x0015  // VM exit due to write access to CR5 (reserved)
#define VMEXIT_CR6_WRITE            0x0016  // VM exit due to write access to CR6 (reserved)
#define VMEXIT_CR7_WRITE            0x0017  // VM exit due to write access to CR7 (reserved)
#define VMEXIT_CR8_WRITE            0x0018  // VM exit due to write access to CR8
#define VMEXIT_CR9_WRITE            0x0019  // VM exit due to write access to CR9 (reserved)
#define VMEXIT_CR10_WRITE           0x001a  // VM exit due to write access to CR10 (reserved)
#define VMEXIT_CR11_WRITE           0x001b  // VM exit due to write access to CR11 (reserved)
#define VMEXIT_CR12_WRITE           0x001c  // VM exit due to write access to CR12 (reserved)
#define VMEXIT_CR13_WRITE           0x001d  // VM exit due to write access to CR13 (reserved)
#define VMEXIT_CR14_WRITE           0x001e  // VM exit due to write access to CR14 (reserved)
#define VMEXIT_CR15_WRITE           0x001f  // VM exit due to write access to CR15 (reserved)

// VM exit codes for reading from debug registers (DR0-DR15)
#define VMEXIT_DR0_READ             0x0020  // VM exit due to read access to DR0
#define VMEXIT_DR1_READ             0x0021  // VM exit due to read access to DR1
#define VMEXIT_DR2_READ             0x0022  // VM exit due to read access to DR2
#define VMEXIT_DR3_READ             0x0023  // VM exit due to read access to DR3
#define VMEXIT_DR4_READ             0x0024  // VM exit due to read access to DR4 (reserved)
#define VMEXIT_DR5_READ             0x0025  // VM exit due to read access to DR5 (reserved)
#define VMEXIT_DR6_READ             0x0026  // VM exit due to read access to DR6
#define VMEXIT_DR7_READ             0x0027  // VM exit due to read access to DR7
#define VMEXIT_DR8_READ             0x0028  // VM exit due to read access to DR8 (reserved)
#define VMEXIT_DR9_READ             0x0029  // VM exit due to read access to DR9 (reserved)
#define VMEXIT_DR10_READ            0x002a  // VM exit due to read access to DR10 (reserved)
#define VMEXIT_DR11_READ            0x002b  // VM exit due to read access to DR11 (reserved)
#define VMEXIT_DR12_READ            0x002c  // VM exit due to read access to DR12 (reserved)
#define VMEXIT_DR13_READ            0x002d  // VM exit due to read access to DR13 (reserved)
#define VMEXIT_DR14_READ            0x002e  // VM exit due to read access to DR14 (reserved)
#define VMEXIT_DR15_READ            0x002f  // VM exit due to read access to DR15 (reserved)

// VM exit codes for writing to debug registers (DR0-DR15)
#define VMEXIT_DR0_WRITE            0x0030  // VM exit due to write access to DR0
#define VMEXIT_DR1_WRITE            0x0031  // VM exit due to write access to DR1
#define VMEXIT_DR2_WRITE            0x0032  // VM exit due to write access to DR2
#define VMEXIT_DR3_WRITE            0x0033  // VM exit due to write access to DR3
#define VMEXIT_DR4_WRITE            0x0034  // VM exit due to write access to DR4 (reserved)
#define VMEXIT_DR5_WRITE            0x0035  // VM exit due to write access to DR5 (reserved)
#define VMEXIT_DR6_WRITE            0x0036  // VM exit due to write access to DR6
#define VMEXIT_DR7_WRITE            0x0037  // VM exit due to write access to DR7
#define VMEXIT_DR8_WRITE            0x0038  // VM exit due to write access to DR8 (reserved)
#define VMEXIT_DR9_WRITE            0x0039  // VM exit due to write access to DR9 (reserved)
#define VMEXIT_DR10_WRITE           0x003a  // VM exit due to write access to DR10 (reserved)
#define VMEXIT_DR11_WRITE           0x003b  // VM exit due to write access to DR11 (reserved)
#define VMEXIT_DR12_WRITE           0x003c  // VM exit due to write access to DR12 (reserved)
#define VMEXIT_DR13_WRITE           0x003d  // VM exit due to write access to DR13 (reserved)
#define VMEXIT_DR14_WRITE           0x003e  // VM exit due to write access to DR14 (reserved)
#define VMEXIT_DR15_WRITE           0x003f  // VM exit due to write access to DR15 (reserved)

// VM exit codes for exceptions and interrupts
#define VMEXIT_EXCEPTION_DE         0x0040  // VM exit due to divide error exception (#DE)
#define VMEXIT_EXCEPTION_DB         0x0041  // VM exit due to debug exception (#DB)
#define VMEXIT_EXCEPTION_NMI        0x0042  // VM exit due to non-maskable interrupt (NMI)
#define VMEXIT_EXCEPTION_BP         0x0043  // VM exit due to breakpoint exception (#BP)
#define VMEXIT_EXCEPTION_OF         0x0044  // VM exit due to overflow exception (#OF)
#define VMEXIT_EXCEPTION_BR         0x0045  // VM exit due to bound range exceeded exception (#BR)
#define VMEXIT_EXCEPTION_UD         0x0046  // VM exit due to undefined opcode exception (#UD)
#define VMEXIT_EXCEPTION_NM         0x0047  // VM exit due to no math coprocessor exception (#NM)
#define VMEXIT_EXCEPTION_DF         0x0048  // VM exit due to double fault exception (#DF)
// Additional exceptions follow the same pattern...

#define VMEXIT_INTR                 0x0060  // VM exit due to external interrupt
#define VMEXIT_NMI                  0x0061  // VM exit due to NMI
#define VMEXIT_SMI                  0x0062  // VM exit due to SMI (System Management Interrupt)
#define VMEXIT_INIT                 0x0063  // VM exit due to INIT signal
#define VMEXIT_VINTR                0x0064  // VM exit due to virtual interrupt
#define VMEXIT_CR0_SEL_WRITE        0x0065  // VM exit due to selective write to CR0
#define VMEXIT_IDTR_READ            0x0066  // VM exit due to read access to IDTR
#define VMEXIT_GDTR_READ            0x0067  // VM exit due to read access to GDTR
#define VMEXIT_LDTR_READ            0x0068  // VM exit due to read access to LDTR
#define VMEXIT_TR_READ              0x0069  // VM exit due to read access to TR
#define VMEXIT_IDTR_WRITE           0x006a  // VM exit due to write access to IDTR
#define VMEXIT_GDTR_WRITE           0x006b  // VM exit due to write access to GDTR
#define VMEXIT_LDTR_WRITE           0x006c  // VM exit due to write access to LDTR
#define VMEXIT_TR_WRITE             0x006d  // VM exit due to write access to TR

// VM exit codes for specific instructions and system events
#define VMEXIT_RDTSC                0x006e  // VM exit due to execution of the RDTSC (Read Time-Stamp Counter) instruction
#define VMEXIT_RDPMC                0x006f  // VM exit due to execution of the RDPMC (Read Performance Monitoring Counter) instruction
#define VMEXIT_PUSHF                0x0070  // VM exit due to execution of the PUSHF (Push Flags Register) instruction
#define VMEXIT_POPF                 0x0071  // VM exit due to execution of the POPF (Pop Flags Register) instruction
#define VMEXIT_CPUID                0x0072  // VM exit due to execution of the CPUID instruction
#define VMEXIT_RSM                  0x0073  // VM exit due to execution of the RSM (Resume from System Management Mode) instruction
#define VMEXIT_IRET                 0x0074  // VM exit due to execution of the IRET (Interrupt Return) instruction
#define VMEXIT_SWINT                0x0075  // VM exit due to execution of a software interrupt
#define VMEXIT_INVD                 0x0076  // VM exit due to execution of the INVD (Invalidate Cache without Writeback) instruction
#define VMEXIT_PAUSE                0x0077  // VM exit due to execution of the PAUSE instruction (typically used in spinlocks)
#define VMEXIT_HLT                  0x0078  // VM exit due to execution of the HLT (Halt) instruction
#define VMEXIT_INVLPG               0x0079  // VM exit due to execution of the INVLPG (Invalidate TLB Entry) instruction
#define VMEXIT_INVLPGA              0x007a  // VM exit due to execution of the INVLPGA instruction (invalidate nested page tables)
#define VMEXIT_IOIO                 0x007b  // VM exit due to an I/O operation (port-based I/O)
#define VMEXIT_MSR                  0x007c  // VM exit due to execution of a MSR (Model-Specific Register) read or write
#define VMEXIT_TASK_SWITCH          0x007d  // VM exit due to a task switch
#define VMEXIT_FERR_FREEZE          0x007e  // VM exit due to FERR (floating-point error) freeze condition
#define VMEXIT_SHUTDOWN             0x007f  // VM exit due to a shutdown event

// VM execution-specific exits
#define VMEXIT_VMRUN                0x0080  // VM exit due to execution of the VMRUN instruction (start a guest)
#define VMEXIT_VMMCALL              0x0081  // VM exit due to execution of the VMMCALL (hypercall) instruction
#define VMEXIT_VMLOAD               0x0082  // VM exit due to execution of the VMLOAD instruction (load a saved VMCB state)
#define VMEXIT_VMSAVE               0x0083  // VM exit due to execution of the VMSAVE instruction (save the current VMCB state)
#define VMEXIT_STGI                 0x0084  // VM exit due to execution of the STGI instruction (set global interrupt flag)
#define VMEXIT_CLGI                 0x0085  // VM exit due to execution of the CLGI instruction (clear global interrupt flag)
#define VMEXIT_SKINIT               0x0086  // VM exit due to execution of the SKINIT (secure initialization) instruction
#define VMEXIT_RDTSCP               0x0087  // VM exit due to execution of the RDTSCP instruction (read time-stamp counter and processor ID)
#define VMEXIT_ICEBP                0x0088  // VM exit due to execution of the ICEBP (debug breakpoint) instruction
#define VMEXIT_WBINVD               0x0089  // VM exit due to execution of the WBINVD (Write Back and Invalidate Cache) instruction
#define VMEXIT_MONITOR              0x008a  // VM exit due to execution of the MONITOR instruction (set up address monitoring for MWAIT)
#define VMEXIT_MWAIT                0x008b  // VM exit due to execution of the MWAIT instruction (monitor wait)
#define VMEXIT_MWAIT_CONDITIONAL    0x008c  // VM exit due to execution of a conditional MWAIT instruction
#define VMEXIT_XSETBV               0x008d  // VM exit due to execution of the XSETBV (set extended control register) instruction

// VM exit codes for trapping write accesses to specific registers
#define VMEXIT_EFER_WRITE_TRAP      0x008f  // VM exit due to an attempt to write to the EFER (Extended Feature Enable Register)
#define VMEXIT_CR0_WRITE_TRAP       0x0090  // VM exit due to an attempt to write to CR0
#define VMEXIT_CR1_WRITE_TRAP       0x0091  // VM exit due to an attempt to write to CR1 (reserved)
#define VMEXIT_CR2_WRITE_TRAP       0x0092  // VM exit due to an attempt to write to CR2
#define VMEXIT_CR3_WRITE_TRAP       0x0093  // VM exit due to an attempt to write to CR3
#define VMEXIT_CR4_WRITE_TRAP       0x0094  // VM exit due to an attempt to write to CR4
#define VMEXIT_CR5_WRITE_TRAP       0x0095  // VM exit due to an attempt to write to CR5 (reserved)
#define VMEXIT_CR6_WRITE_TRAP       0x0096  // VM exit due to an attempt to write to CR6 (reserved)
#define VMEXIT_CR7_WRITE_TRAP       0x0097  // VM exit due to an attempt to write to CR7 (reserved)
#define VMEXIT_CR8_WRITE_TRAP       0x0098  // VM exit due to an attempt to write to CR8
#define VMEXIT_CR9_WRITE_TRAP       0x0099  // VM exit due to an attempt to write to CR9 (reserved)
#define VMEXIT_CR10_WRITE_TRAP      0x009a  // VM exit due to an attempt to write to CR10 (reserved)
#define VMEXIT_CR11_WRITE_TRAP      0x009b  // VM exit due to an attempt to write to CR11 (reserved)
#define VMEXIT_CR12_WRITE_TRAP      0x009c  // VM exit due to an attempt to write to CR12 (reserved)
#define VMEXIT_CR13_WRITE_TRAP      0x009d  // VM exit due to an attempt to write to CR13 (reserved)
#define VMEXIT_CR14_WRITE_TRAP      0x009e  // VM exit due to an attempt to write to CR14 (reserved)
#define VMEXIT_CR15_WRITE_TRAP      0x009f  // VM exit due to an attempt to write to CR15 (reserved)

// Nested Page Fault and other VM exit codes
#define VMEXIT_NPF                  0x0400  // VM exit due to a nested page fault (guest page fault in nested paging mode)
#define AVIC_INCOMPLETE_IPI         0x0401  // VM exit due to incomplete IPI delivery in AVIC (Advanced Virtual Interrupt Controller)
#define AVIC_NOACCEL                0x0402  // VM exit due to lack of acceleration in AVIC
#define VMEXIT_VMGEXIT              0x0403  // VM exit due to a VMGEXIT instruction (for secure nested paging)
#define VMEXIT_INVALID              -1      // Invalid VM exit cod



// Interrupt Types for Exception Handling
enum _INTERRUPT_TYPE
{
    INTERRUPT_TYPE_EXTERNAL_INTERRUPT = 0,           // External interrupt
    INTERRUPT_TYPE_RESERVED = 1,                     // Reserved type
    INTERRUPT_TYPE_NMI = 2,                          // Non-maskable interrupt
    INTERRUPT_TYPE_HARDWARE_EXCEPTION = 3,           // Hardware exception
    INTERRUPT_TYPE_SOFTWARE_INTERRUPT = 4,           // Software interrupt
    INTERRUPT_TYPE_PRIVILEGED_SOFTWARE_INTERRUPT = 5, // Privileged software interrupt
    INTERRUPT_TYPE_SOFTWARE_EXCEPTION = 6,           // Software exception
    INTERRUPT_TYPE_OTHER_EVENT = 7                   // Other event
};

// Exception Vectors for Different Exception Types
enum _EXCEPTION_VECTORS
{
    EXCEPTION_VECTOR_DIVIDE_ERROR,                   // Divide error (#DE)
    EXCEPTION_VECTOR_DEBUG_BREAKPOINT,               // Debug breakpoint (#DB)
    EXCEPTION_VECTOR_NMI,                            // Non-maskable interrupt (#NMI)
    EXCEPTION_VECTOR_BREAKPOINT,                     // Breakpoint (#BP)
    EXCEPTION_VECTOR_OVERFLOW,                       // Overflow (#OF)
    EXCEPTION_VECTOR_BOUND_RANGE_EXCEEDED,           // Bound range exceeded (#BR)
    EXCEPTION_VECTOR_UNDEFINED_OPCODE,               // Undefined opcode (#UD)
    EXCEPTION_VECTOR_NO_MATH_COPROCESSOR,            // No math coprocessor
    EXCEPTION_VECTOR_DOUBLE_FAULT,                   // Double fault (#DF)
    EXCEPTION_VECTOR_RESERVED0,                      // Reserved exception
    EXCEPTION_VECTOR_INVALID_TASK_SEGMENT_SELECTOR,  // Invalid TSS (#TS)
    EXCEPTION_VECTOR_SEGMENT_NOT_PRESENT,            // Segment not present (#NP)
    EXCEPTION_VECTOR_STACK_SEGMENT_FAULT,            // Stack segment fault (#SS)
    EXCEPTION_VECTOR_GENERAL_PROTECTION_FAULT,       // General protection fault (#GP)
    EXCEPTION_VECTOR_PAGE_FAULT,                     // Page fault (#PF)
    EXCEPTION_VECTOR_RESERVED1,                      // Reserved exception
    EXCEPTION_VECTOR_MATH_FAULT,                     // Math fault (#MF)
    EXCEPTION_VECTOR_ALIGNMENT_CHECK,                // Alignment check (#AC)
    EXCEPTION_VECTOR_MACHINE_CHECK,                  // Machine check (#MC)
    EXCEPTION_VECTOR_SIMD_FLOATING_POINT_NUMERIC_ERROR, // SIMD floating-point numeric error (#XF)
    EXCEPTION_VECTOR_VIRTUAL_EXCEPTION,              // Virtualization exception
    EXCEPTION_VECTOR_RESERVED2,                      // Reserved
    // Additional reserved vectors
    EXCEPTION_VECTOR_RESERVED9,
    EXCEPTION_VECTOR_RESERVED10,
    EXCEPTION_VECTOR_RESERVED11,
    EXCEPTION_VECTOR_RESERVED12,

    // Windows Specific Exception Vectors
    APC_INTERRUPT = 31,                              // Asynchronous Procedure Call
    DPC_INTERRUPT = 47,                              // Deferred Procedure Call
    CLOCK_INTERRUPT = 209,                           // Clock interrupt
    IPI_INTERRUPT = 225,                             // Inter-Processor Interrupt
    PMI_INTERRUPT = 254                              // Performance Monitoring Interrupt
};

#endif // !DEFINES_HPP