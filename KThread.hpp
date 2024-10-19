#ifndef KTHREAD_HPP
#define KTHREAD_HPP
#include "UndefinedStructs.hpp"


// https://www.vergiliusproject.com/kernels/x64/windows-11/23h2

//0x480 bytes (sizeof)
struct __KTHREAD
{
    struct _DISPATCHER_HEADER Header;                                       //0x0
    void* SListFaultAddress;                                                //0x18
    uintmax_t QuantumTarget;                                                //0x20
    void* InitialStack;                                                     //0x28
    void* volatile StackLimit;                                              //0x30
    void* StackBase;                                                        //0x38
    uintmax_t ThreadLock;                                                   //0x40
    volatile uintmax_t CycleTime;                                           //0x48
    ulong_t CurrentRunTime;                                                   //0x50
    ulong_t ExpectedRunTime;                                                  //0x54
    void* KernelStack;                                                      //0x58
    struct _XSAVE_FORMAT* StateSaveArea;                                    //0x60
    struct _KSCHEDULING_GROUP* volatile SchedulingGroup;                    //0x68
    union _KWAIT_STATUS_REGISTER WaitRegister;                              //0x70
    volatile uint8_t Running;                                                 //0x71
    uint8_t Alerted[2];                                                       //0x72
    union
    {
        struct
        {
            ulong_t AutoBoostActive : 1;                                        //0x74
            ulong_t ReadyTransition : 1;                                        //0x74
            ulong_t WaitNext : 1;                                               //0x74
            ulong_t SystemAffinityActive : 1;                                   //0x74
            ulong_t Alertable : 1;                                              //0x74
            ulong_t UserStackWalkActive : 1;                                    //0x74
            ulong_t ApcInterruptRequest : 1;                                    //0x74
            ulong_t QuantumEndMigrate : 1;                                      //0x74
            ulong_t SecureThread : 1;                                           //0x74
            ulong_t TimerActive : 1;                                            //0x74
            ulong_t SystemThread : 1;                                           //0x74
            ulong_t ProcessDetachActive : 1;                                    //0x74
            ulong_t CalloutActive : 1;                                          //0x74
            ulong_t ScbReadyQueue : 1;                                          //0x74
            ulong_t ApcQueueable : 1;                                           //0x74
            ulong_t ReservedStackInUse : 1;                                     //0x74
            ulong_t Spare : 1;                                                  //0x74
            ulong_t TimerSuspended : 1;                                         //0x74
            ulong_t SuspendedWaitMode : 1;                                      //0x74
            ulong_t SuspendSchedulerApcWait : 1;                                //0x74
            ulong_t CetUserShadowStack : 1;                                     //0x74
            ulong_t BypassProcessFreeze : 1;                                    //0x74
            ulong_t CetKernelShadowStack : 1;                                   //0x74
            ulong_t StateSaveAreaDecoupled : 1;                                 //0x74
            ulong_t Reserved : 8;                                               //0x74
        };
        long MiscFlags;                                                     //0x74
    };
    union
    {
        struct
        {
            ulong_t UserIdealProcessorFixed : 1;                                //0x78
            ulong_t IsolationWidth : 1;                                         //0x78
            ulong_t AutoAlignment : 1;                                          //0x78
            ulong_t DisableBoost : 1;                                           //0x78
            ulong_t AlertedByThreadId : 1;                                      //0x78
            ulong_t QuantumDonation : 1;                                        //0x78
            ulong_t EnableStackSwap : 1;                                        //0x78
            ulong_t GuiThread : 1;                                              //0x78
            ulong_t DisableQuantum : 1;                                         //0x78
            ulong_t ChargeOnlySchedulingGroup : 1;                              //0x78
            ulong_t DeferPreemption : 1;                                        //0x78
            ulong_t QueueDeferPreemption : 1;                                   //0x78
            ulong_t ForceDeferSchedule : 1;                                     //0x78
            ulong_t SharedReadyQueueAffinity : 1;                               //0x78
            ulong_t FreezeCount : 1;                                            //0x78
            ulong_t TerminationApcRequest : 1;                                  //0x78
            ulong_t AutoBoostEntriesExhausted : 1;                              //0x78
            ulong_t KernelStackResident : 1;                                    //0x78
            ulong_t TerminateRequestReason : 2;                                 //0x78
            ulong_t ProcessStackCountDecremented : 1;                           //0x78
            ulong_t RestrictedGuiThread : 1;                                    //0x78
            ulong_t VpBackingThread : 1;                                        //0x78
            ulong_t EtwStackTraceCrimsonApcDisabled : 1;                        //0x78
            ulong_t EtwStackTraceApcInserted : 8;                               //0x78
        };
        volatile long ThreadFlags;                                          //0x78
    };
    volatile uint8_t Tag;                                                     //0x7c
    uint8_t SystemHeteroCpuPolicy;                                            //0x7d
    uint8_t UserHeteroCpuPolicy : 7;                                            //0x7e
    uint8_t ExplicitSystemHeteroCpuPolicy : 1;                                  //0x7e
    union
    {
        struct
        {
            uint8_t RunningNonRetpolineCode : 1;                                //0x7f
            uint8_t SpecCtrlSpare : 7;                                          //0x7f
        };
        uint8_t SpecCtrl;                                                     //0x7f
    };
    ulong_t SystemCallNumber;                                                 //0x80
    ulong_t ReadyTime;                                                        //0x84
    void* FirstArgument;                                                    //0x88
    struct _KTRAP_FRAME* TrapFrame;                                         //0x90
    union
    {
        struct __KAPC_STATE ApcState;                                        //0x98
        struct
        {
            uint8_t ApcStateFill[43];                                         //0x98
            int8_t Priority;                                                  //0xc3
            ulong_t UserIdealProcessor;                                       //0xc4
        };
    };
    volatile intmax_t WaitStatus;                                           //0xc8
    struct _KWAIT_BLOCK* WaitBlockList;                                     //0xd0
    union
    {
        struct _LIST_ENTRY WaitListEntry;                                   //0xd8
        struct _SINGLE_LIST_ENTRY SwapListEntry;                            //0xd8
    };
    struct _DISPATCHER_HEADER* volatile Queue;                              //0xe8
    void* Teb;                                                              //0xf0
    uintmax_t RelativeTimerBias;                                            //0xf8
    struct _KTIMER Timer;                                                   //0x100
    union
    {
        struct _KWAIT_BLOCK WaitBlock[4];                                   //0x140
        struct
        {
            uint8_t WaitBlockFill4[20];                                       //0x140
            ulong_t ContextSwitches;                                          //0x154
        };
        struct
        {
            uint8_t WaitBlockFill5[68];                                       //0x140
            volatile uint8_t State;                                           //0x184
            int8_t Spare13;                                                   //0x185
            uint8_t WaitIrql;                                                 //0x186
            int8_t WaitMode;                                                  //0x187
        };
        struct
        {
            uint8_t WaitBlockFill6[116];                                      //0x140
            ulong_t WaitTime;                                                 //0x1b4
        };
        struct
        {
            uint8_t WaitBlockFill7[164];                                      //0x140
            union
            {
                struct
                {
                    SHORT KernelApcDisable;                                 //0x1e4
                    SHORT SpecialApcDisable;                                //0x1e6
                };
                ulong_t CombinedApcDisable;                                   //0x1e4
            };
        };
        struct
        {
            uint8_t WaitBlockFill8[40];                                       //0x140
            struct _KTHREAD_COUNTERS* ThreadCounters;                       //0x168
        };
        struct
        {
            uint8_t WaitBlockFill9[88];                                       //0x140
            struct _XSTATE_SAVE* XStateSave;                                //0x198
        };
        struct
        {
            uint8_t WaitBlockFill10[136];                                     //0x140
            void* volatile Win32Thread;                                     //0x1c8
        };
        struct
        {
            uint8_t WaitBlockFill11[176];                                     //0x140
            uintmax_t Spare18;                                              //0x1f0
            uintmax_t Spare19;                                              //0x1f8
        };
    };
    union
    {
        volatile long ThreadFlags2;                                         //0x200
        struct
        {
            ulong_t BamQosLevel : 8;                                            //0x200
            ulong_t ThreadFlags2Reserved : 24;                                  //0x200
        };
    };
    uint8_t HgsFeedbackClass;                                                 //0x204
    uint8_t Spare23[3];                                                       //0x205
    struct _LIST_ENTRY QueueListEntry;                                      //0x208
    union
    {
        volatile ulong_t NextProcessor;                                       //0x218
        struct
        {
            ulong_t NextProcessorNumber : 31;                                   //0x218
            ulong_t SharedReadyQueue : 1;                                       //0x218
        };
    };
    long QueuePriority;                                                     //0x21c
    struct _KPROCESS* Process;                                              //0x220
    struct _KAFFINITY_EX* UserAffinity;                                     //0x228
    USHORT UserAffinityPrimaryGroup;                                        //0x230
    int8_t PreviousMode;                                                      //0x232
    int8_t BasePriority;                                                      //0x233
    union
    {
        int8_t PriorityDecrement;                                             //0x234
        struct
        {
            uint8_t ForegroundBoost : 4;                                        //0x234
            uint8_t UnusualBoost : 4;                                           //0x234
        };
    };
    uint8_t Preempted;                                                        //0x235
    uint8_t AdjustReason;                                                     //0x236
    int8_t AdjustIncrement;                                                   //0x237
    uintmax_t AffinityVersion;                                              //0x238
    struct _KAFFINITY_EX* Affinity;                                         //0x240
    USHORT AffinityPrimaryGroup;                                            //0x248
    uint8_t ApcStateIndex;                                                    //0x24a
    uint8_t WaitBlockCount;                                                   //0x24b
    ulong_t IdealProcessor;                                                   //0x24c
    uintmax_t NpxState;                                                     //0x250
    union
    {
        struct __KAPC_STATE SavedApcState;                                   //0x258
        struct
        {
            uint8_t SavedApcStateFill[43];                                    //0x258
            uint8_t WaitReason;                                               //0x283
            int8_t SuspendCount;                                              //0x284
            int8_t Saturation;                                                //0x285
            USHORT SListFaultCount;                                         //0x286
        };
    };
    union
    {
        struct _KAPC SchedulerApc;                                          //0x288
        struct
        {
            uint8_t SchedulerApcFill1[3];                                     //0x288
            uint8_t QuantumReset;                                             //0x28b
        };
        struct
        {
            uint8_t SchedulerApcFill2[4];                                     //0x288
            ulong_t KernelTime;                                               //0x28c
        };
        struct
        {
            uint8_t SchedulerApcFill3[64];                                    //0x288
            struct _KPRCB* volatile WaitPrcb;                               //0x2c8
        };
        struct
        {
            uint8_t SchedulerApcFill4[72];                                    //0x288
            void* LegoData;                                                 //0x2d0
        };
        struct
        {
            uint8_t SchedulerApcFill5[83];                                    //0x288
            uint8_t CallbackNestingLevel;                                     //0x2db
            ulong_t UserTime;                                                 //0x2dc
        };
    };
    struct _KEVENT SuspendEvent;                                            //0x2e0
    struct _LIST_ENTRY ThreadListEntry;                                     //0x2f8
    struct _LIST_ENTRY MutantListHead;                                      //0x308
    uint8_t AbEntrySummary;                                                   //0x318
    uint8_t AbWaitEntryCount;                                                 //0x319
    union
    {
        uint8_t FreezeFlags;                                                  //0x31a
        struct
        {
            uint8_t FreezeCount2 : 1;                                           //0x31a
            uint8_t FreezeNormal : 1;                                           //0x31a
            uint8_t FreezeDeep : 1;                                             //0x31a
        };
    };
    int8_t SystemPriority;                                                    //0x31b
    ulong_t SecureThreadCookie;                                               //0x31c
    void* Spare22;                                                          //0x320
    struct _SINGLE_LIST_ENTRY PropagateBoostsEntry;                         //0x328
    struct _SINGLE_LIST_ENTRY IoSelfBoostsEntry;                            //0x330
    uint8_t PriorityFloorCounts[32];                                          //0x338
    ulong_t PriorityFloorSummary;                                             //0x358
    volatile long AbCompletedIoBoostCount;                                  //0x35c
    volatile long AbCompletedIoQoSBoostCount;                               //0x360
    volatile SHORT KeReferenceCount;                                        //0x364
    uint8_t AbOrphanedEntrySummary;                                           //0x366
    uint8_t AbOwnedEntryCount;                                                //0x367
    ulong_t ForegroundLossTime;                                               //0x368
    union
    {
        struct _LIST_ENTRY GlobalForegroundListEntry;                       //0x370
        struct
        {
            struct _SINGLE_LIST_ENTRY ForegroundDpcStackListEntry;          //0x370
            uintmax_t InGlobalForegroundList;                               //0x378
        };
    };
    intmax_t ReadOperationCount;                                            //0x380
    intmax_t WriteOperationCount;                                           //0x388
    intmax_t OtherOperationCount;                                           //0x390
    intmax_t ReadTransferCount;                                             //0x398
    intmax_t WriteTransferCount;                                            //0x3a0
    intmax_t OtherTransferCount;                                            //0x3a8
    struct _KSCB* QueuedScb;                                                //0x3b0
    volatile ulong_t ThreadTimerDelay;                                        //0x3b8
    union
    {
        volatile long ThreadFlags3;                                         //0x3bc
        struct
        {
            ulong_t ThreadFlags3Reserved : 8;                                   //0x3bc
            ulong_t PpmPolicy : 3;                                              //0x3bc
            ulong_t ThreadFlags3Reserved2 : 21;                                 //0x3bc
        };
    };
    uintmax_t TracingPrivate[1];                                            //0x3c0
    void* SchedulerAssist;                                                  //0x3c8
    void* volatile AbWaitObject;                                            //0x3d0
    ulong_t ReservedPreviousReadyTimeValue;                                   //0x3d8
    uintmax_t KernelWaitTime;                                               //0x3e0
    uintmax_t UserWaitTime;                                                 //0x3e8
    union
    {
        struct _LIST_ENTRY GlobalUpdateVpThreadPriorityListEntry;           //0x3f0
        struct
        {
            struct _SINGLE_LIST_ENTRY UpdateVpThreadPriorityDpcStackListEntry; //0x3f0
            uintmax_t InGlobalUpdateVpThreadPriorityList;                   //0x3f8
        };
    };
    long SchedulerAssistPriorityFloor;                                      //0x400
    long RealtimePriorityFloor;                                             //0x404
    void* KernelShadowStack;                                                //0x408
    void* KernelShadowStackInitial;                                         //0x410
    void* KernelShadowStackBase;                                            //0x418
    union _KERNEL_SHADOW_STACK_LIMIT KernelShadowStackLimit;                //0x420
    uintmax_t ExtendedFeatureDisableMask;                                   //0x428
    uintmax_t HgsFeedbackStartTime;                                         //0x430
    uintmax_t HgsFeedbackCycles;                                            //0x438
    ulong_t HgsInvalidFeedbackCount;                                          //0x440
    ulong_t HgsLowerPerfClassFeedbackCount;                                   //0x444
    ulong_t HgsHigherPerfClassFeedbackCount;                                  //0x448
    ulong_t Spare27;                                                          //0x44c
    struct _SINGLE_LIST_ENTRY SystemAffinityTokenListHead;                  //0x450
    void* IptSaveArea;                                                      //0x458
    uint8_t ResourceIndex;                                                    //0x460
    uint8_t CoreIsolationReasons;                                             //0x461
    uint8_t BamQosLevelFromAssistPage;                                        //0x462
    uint8_t Spare31[1];                                                       //0x463
    ulong_t Spare32;                                                          //0x464
    uintmax_t EndPadding[3];                                                //0x468
};




#endif // !KTHREAD_HPP

