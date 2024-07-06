#ifndef EPROCESS_HPP
#define EPROCESS_HPP
#include "UndefinedStructs.hpp"

// https://www.vergiliusproject.com/kernels/x64/windows-11/23h2

//0x438 bytes (sizeof)
struct __KPROCESS
{
    struct _DISPATCHER_HEADER Header;                                       //0x0
    struct _LIST_ENTRY ProfileListHead;                                     //0x18
    uintmax_t DirectoryTableBase;                                           //0x28
    struct _LIST_ENTRY ThreadListHead;                                      //0x30   
    uint8_t padding0[0x350 - 0x40];
    struct _LIST_ENTRY ProcessListEntry;                                    //0x350
    uintmax_t CycleTime;                                                    //0x360
    uintmax_t ContextSwitches;                                              //0x368
    struct _KSCHEDULING_GROUP* SchedulingGroup;                             //0x370
    ulong_t FreezeCount;                                                      //0x378
    ulong_t KernelTime;                                                       //0x37c
    ulong_t UserTime;                                                         //0x380
    ulong_t ReadyTime;                                                        //0x384
    uintmax_t UserDirectoryTableBase;                                       //0x388
    uint8_t AddressPolicy;                                                    //0x390
    uint8_t Spare2[71];                                                       //0x391
    void* InstrumentationCallback;                                          //0x3d8
    union
    {
        uintmax_t SecureHandle;                                             //0x3e0
        struct
        {
            uintmax_t SecureProcess : 1;                                      //0x3e0
            uintmax_t Unused : 1;                                             //0x3e0
        } Flags;                                                            //0x3e0
    } SecureState;                                                          //0x3e0
    uintmax_t KernelWaitTime;                                               //0x3e8
    uintmax_t UserWaitTime;                                                 //0x3f0
    uintmax_t LastRebalanceQpc;                                             //0x3f8
    void* PerProcessorCycleTimes;                                           //0x400
    uintmax_t ExtendedFeatureDisableMask;                                   //0x408
    uint16_t PrimaryGroup;                                                    //0x410
    uint16_t Spare3[3];                                                       //0x412
    void* UserCetLogging;                                                   //0x418
    struct _LIST_ENTRY CpuPartitionList;                                    //0x420
    uintmax_t EndPadding[1];                                                //0x430
};







//0xb80 bytes (sizeof)
struct __EPROCESS
{
    struct __KPROCESS Pcb;                                                   //0x0
    struct _EX_PUSH_LOCK ProcessLock;                                       //0x438
    void* UniqueProcessId;                                                  //0x440
    struct _LIST_ENTRY ActiveProcessLinks;                                  //0x448
    struct _EX_RUNDOWN_REF RundownProtect;                                  //0x458
    union
    {
        ulong_t Flags2;                                                       //0x460
        struct
        {
            ulong_t JobNotReallyActive : 1;                                     //0x460
            ulong_t AccountingFolded : 1;                                       //0x460
            ulong_t NewProcessReported : 1;                                     //0x460
            ulong_t ExitProcessReported : 1;                                    //0x460
            ulong_t ReportCommitChanges : 1;                                    //0x460
            ulong_t LastReportMemory : 1;                                       //0x460
            ulong_t ForceWakeCharge : 1;                                        //0x460
            ulong_t CrossSessionCreate : 1;                                     //0x460
            ulong_t NeedsHandleRundown : 1;                                     //0x460
            ulong_t RefTraceEnabled : 1;                                        //0x460
            ulong_t PicoCreated : 1;                                            //0x460
            ulong_t EmptyJobEvaluated : 1;                                      //0x460
            ulong_t DefaultPagePriority : 3;                                    //0x460
            ulong_t PrimaryTokenFrozen : 1;                                     //0x460
            ulong_t ProcessVerifierTarget : 1;                                  //0x460
            ulong_t RestrictSetThreadContext : 1;                               //0x460
            ulong_t AffinityPermanent : 1;                                      //0x460
            ulong_t AffinityUpdateEnable : 1;                                   //0x460
            ulong_t PropagateNode : 1;                                          //0x460
            ulong_t ExplicitAffinity : 1;                                       //0x460
            ulong_t ProcessExecutionState : 2;                                  //0x460
            ulong_t EnableReadVmLogging : 1;                                    //0x460
            ulong_t EnableWriteVmLogging : 1;                                   //0x460
            ulong_t FatalAccessTerminationRequested : 1;                        //0x460
            ulong_t DisableSystemAllowedCpuSet : 1;                             //0x460
            ulong_t ProcessStateChangeRequest : 2;                              //0x460
            ulong_t ProcessStateChangeInProgress : 1;                           //0x460
            ulong_t InPrivate : 1;                                              //0x460
        };
    };
    union
    {
        ulong_t Flags;                                                        //0x464
        struct
        {
            ulong_t CreateReported : 1;                                         //0x464
            ulong_t NoDebugInherit : 1;                                         //0x464
            ulong_t ProcessExiting : 1;                                         //0x464
            ulong_t ProcessDelete : 1;                                          //0x464
            ulong_t ManageExecutableMemoryWrites : 1;                           //0x464
            ulong_t VmDeleted : 1;                                              //0x464
            ulong_t OutswapEnabled : 1;                                         //0x464
            ulong_t Outswapped : 1;                                             //0x464
            ulong_t FailFastOnCommitFail : 1;                                   //0x464
            ulong_t Wow64VaSpace4Gb : 1;                                        //0x464
            ulong_t AddressSpaceInitialized : 2;                                //0x464
            ulong_t SetTimerResolution : 1;                                     //0x464
            ulong_t BreakOnTermination : 1;                                     //0x464
            ulong_t DeprioritizeViews : 1;                                      //0x464
            ulong_t WriteWatch : 1;                                             //0x464
            ulong_t ProcessInSession : 1;                                       //0x464
            ulong_t OverrideAddressSpace : 1;                                   //0x464
            ulong_t HasAddressSpace : 1;                                        //0x464
            ulong_t LaunchPrefetched : 1;                                       //0x464
            ulong_t Reserved : 1;                                               //0x464
            ulong_t VmTopDown : 1;                                              //0x464
            ulong_t ImageNotifyDone : 1;                                        //0x464
            ulong_t PdeUpdateNeeded : 1;                                        //0x464
            ulong_t VdmAllowed : 1;                                             //0x464
            ulong_t ProcessRundown : 1;                                         //0x464
            ulong_t ProcessInserted : 1;                                        //0x464
            ulong_t DefaultIoPriority : 3;                                      //0x464
            ulong_t ProcessSelfDelete : 1;                                      //0x464
            ulong_t SetTimerResolutionLink : 1;                                 //0x464
        };
    };
    union _LARGE_INTEGER CreateTime;                                        //0x468
    uintmax_t ProcessQuotaUsage[2];                                         //0x470
    uintmax_t ProcessQuotaPeak[2];                                          //0x480
    uintmax_t PeakVirtualSize;                                              //0x490
    uintmax_t VirtualSize;                                                  //0x498
    struct _LIST_ENTRY SessionProcessLinks;                                 //0x4a0
    union
    {
        void* ExceptionPortData;                                            //0x4b0
        uintmax_t ExceptionPortValue;                                       //0x4b0
        uintmax_t ExceptionPortState : 3;                                     //0x4b0
    };
    struct _EX_FAST_REF Token;                                              //0x4b8
    uintmax_t MmReserved;                                                   //0x4c0
    struct _EX_PUSH_LOCK AddressCreationLock;                               //0x4c8
    struct _EX_PUSH_LOCK PageTableCommitmentLock;                           //0x4d0
    struct _ETHREAD* RotateInProgress;                                      //0x4d8
    struct _ETHREAD* ForkInProgress;                                        //0x4e0
    struct _EJOB* volatile CommitChargeJob;                                 //0x4e8
    struct _RTL_AVL_TREE CloneRoot;                                         //0x4f0
    volatile uintmax_t NumberOfPrivatePages;                                //0x4f8
    volatile uintmax_t NumberOfLockedPages;                                 //0x500
    void* Win32Process;                                                     //0x508
    struct _EJOB* volatile Job;                                             //0x510
    void* SectionObject;                                                    //0x518
    void* SectionBaseAddress;                                               //0x520
    ulong_t Cookie;                                                           //0x528
    struct _PAGEFAULT_HISTORY* WorkingSetWatch;                             //0x530
    void* Win32WindowStation;                                               //0x538
    void* InheritedFromUniqueProcessId;                                     //0x540
    volatile uintmax_t OwnerProcessId;                                      //0x548
    struct _PEB* Peb;                                                       //0x550
    struct _MM_SESSION_SPACE* Session;                                      //0x558
    void* Spare1;                                                           //0x560
    struct _EPROCESS_QUOTA_BLOCK* QuotaBlock;                               //0x568
    struct _HANDLE_TABLE* ObjectTable;                                      //0x570
    void* DebugPort;                                                        //0x578
    struct _EWOW64PROCESS* WoW64Process;                                    //0x580
    struct _EX_FAST_REF DeviceMap;                                          //0x588
    void* EtwDataSource;                                                    //0x590
    uintmax_t PageDirectoryPte;                                             //0x598
    struct _FILE_OBJECT* ImageFilePointer;                                  //0x5a0
    uint8_t ImageFileName[15];                                                //0x5a8
    uint8_t PriorityClass;                                                    //0x5b7
    void* SecurityPort;                                                     //0x5b8
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;      //0x5c0
    struct _LIST_ENTRY JobLinks;                                            //0x5c8
    void* HighestUserAddress;                                               //0x5d8
    struct _LIST_ENTRY ThreadListHead;                                      //0x5e0
    volatile ulong_t ActiveThreads;                                           //0x5f0
    ulong_t ImagePathHash;                                                    //0x5f4
    ulong_t DefaultHardErrorProcessing;                                       //0x5f8
    long LastThreadExitStatus;                                              //0x5fc
    struct _EX_FAST_REF PrefetchTrace;                                      //0x600
    void* LockedPagesList;                                                  //0x608
    union _LARGE_INTEGER ReadOperationCount;                                //0x610
    union _LARGE_INTEGER WriteOperationCount;                               //0x618
    union _LARGE_INTEGER OtherOperationCount;                               //0x620
    union _LARGE_INTEGER ReadTransferCount;                                 //0x628
    union _LARGE_INTEGER WriteTransferCount;                                //0x630
    union _LARGE_INTEGER OtherTransferCount;                                //0x638
    uintmax_t CommitChargeLimit;                                            //0x640
    volatile uintmax_t CommitCharge;                                        //0x648
    volatile uintmax_t CommitChargePeak;                                    //0x650
    struct _MMSUPPORT_FULL Vm;                                              //0x680
    struct _LIST_ENTRY MmProcessLinks;                                      //0x7c0
    ulong_t ModifiedPageCount;                                                //0x7d0
    long ExitStatus;                                                        //0x7d4
    struct _RTL_AVL_TREE VadRoot;                                           //0x7d8
    void* VadHint;                                                          //0x7e0
    uintmax_t VadCount;                                                     //0x7e8
    volatile uintmax_t VadPhysicalPages;                                    //0x7f0
    uintmax_t VadPhysicalPagesLimit;                                        //0x7f8
    struct _ALPC_PROCESS_CONTEXT AlpcContext;                               //0x800
    struct _LIST_ENTRY TimerResolutionLink;                                 //0x820
    struct _PO_DIAG_STACK_RECORD* TimerResolutionStackRecord;               //0x830
    ulong_t RequestedTimerResolution;                                         //0x838
    ulong_t SmallestTimerResolution;                                          //0x83c
    union _LARGE_INTEGER ExitTime;                                          //0x840
    struct _INVERTED_FUNCTION_TABLE_KERNEL_MODE* InvertedFunctionTable;     //0x848
    struct _EX_PUSH_LOCK InvertedFunctionTableLock;                         //0x850
    ulong_t ActiveThreadsHighWatermark;                                       //0x858
    ulong_t LargePrivateVadCount;                                             //0x85c
    struct _EX_PUSH_LOCK ThreadListLock;                                    //0x860
    void* WnfContext;                                                       //0x868
    struct _EJOB* ServerSilo;                                               //0x870
    uint8_t SignatureLevel;                                                   //0x878
    uint8_t SectionSignatureLevel;                                            //0x879
    struct _PS_PROTECTION Protection;                                       //0x87a
    uint8_t HangCount : 3;                                                      //0x87b
    uint8_t GhostCount : 3;                                                     //0x87b
    uint8_t PrefilterException : 1;                                             //0x87b
    union
    {
        ulong_t Flags3;                                                       //0x87c
        struct
        {
            ulong_t Minimal : 1;                                                //0x87c
            ulong_t ReplacingPageRoot : 1;                                      //0x87c
            ulong_t Crashed : 1;                                                //0x87c
            ulong_t JobVadsAreTracked : 1;                                      //0x87c
            ulong_t VadTrackingDisabled : 1;                                    //0x87c
            ulong_t AuxiliaryProcess : 1;                                       //0x87c
            ulong_t SubsystemProcess : 1;                                       //0x87c
            ulong_t IndirectCpuSets : 1;                                        //0x87c
            ulong_t RelinquishedCommit : 1;                                     //0x87c
            ulong_t HighGraphicsPriority : 1;                                   //0x87c
            ulong_t CommitFailLogged : 1;                                       //0x87c
            ulong_t ReserveFailLogged : 1;                                      //0x87c
            ulong_t SystemProcess : 1;                                          //0x87c
            ulong_t HideImageBaseAddresses : 1;                                 //0x87c
            ulong_t AddressPolicyFrozen : 1;                                    //0x87c
            ulong_t ProcessFirstResume : 1;                                     //0x87c
            ulong_t ForegroundExternal : 1;                                     //0x87c
            ulong_t ForegroundSystem : 1;                                       //0x87c
            ulong_t HighMemoryPriority : 1;                                     //0x87c
            ulong_t EnableProcessSuspendResumeLogging : 1;                      //0x87c
            ulong_t EnableThreadSuspendResumeLogging : 1;                       //0x87c
            ulong_t SecurityDomainChanged : 1;                                  //0x87c
            ulong_t SecurityFreezeComplete : 1;                                 //0x87c
            ulong_t VmProcessorHost : 1;                                        //0x87c
            ulong_t VmProcessorHostTransition : 1;                              //0x87c
            ulong_t AltSyscall : 1;                                             //0x87c
            ulong_t TimerResolutionIgnore : 1;                                  //0x87c
            ulong_t DisallowUserTerminate : 1;                                  //0x87c
            ulong_t EnableProcessRemoteExecProtectVmLogging : 1;                //0x87c
            ulong_t EnableProcessLocalExecProtectVmLogging : 1;                 //0x87c
            ulong_t MemoryCompressionProcess : 1;                               //0x87c
        };
    };
    long DeviceAsid;                                                        //0x880
    void* SvmData;                                                          //0x888
    struct _EX_PUSH_LOCK SvmProcessLock;                                    //0x890
    uintmax_t SvmLock;                                                      //0x898
    struct _LIST_ENTRY SvmProcessDeviceListHead;                            //0x8a0
    uintmax_t LastFreezeInterruptTime;                                      //0x8b0
    struct _PROCESS_DISK_COUNTERS* DiskCounters;                            //0x8b8
    void* PicoContext;                                                      //0x8c0
    void* EnclaveTable;                                                     //0x8c8
    uintmax_t EnclaveNumber;                                                //0x8d0
    struct _EX_PUSH_LOCK EnclaveLock;                                       //0x8d8
    ulong_t HighPriorityFaultsAllowed;                                        //0x8e0
    struct _PO_PROCESS_ENERGY_CONTEXT* EnergyContext;                       //0x8e8
    void* VmContext;                                                        //0x8f0
    uintmax_t SequenceNumber;                                               //0x8f8
    uintmax_t CreateInterruptTime;                                          //0x900
    uintmax_t CreateUnbiasedInterruptTime;                                  //0x908
    uintmax_t TotalUnbiasedFrozenTime;                                      //0x910
    uint8_t padding0[0xb70 - 0x910];
};



#endif// !EPROCESS_HPP