#ifndef UNDEFINEDSTRUCTS_HPP
#define UNDEFINEDSTRUCTS_HPP

// https://www.vergiliusproject.com/kernels/x64/windows-11/23h2

#include <cstdint>
#include <ntdef.h>
#include <ntddk.h>

using  ulong_t = unsigned long;



//0x8 bytes (sizeof)
struct _SE_AUDIT_PROCESS_CREATION_INFO
{
    struct _OBJECT_NAME_INFORMATION* ImageFileName;                         //0x0
};



//0x30 bytes (sizeof)
struct __KAPC_STATE
{
    struct _LIST_ENTRY ApcListHead[2];                                      //0x0
    struct _KPROCESS* Process;                                              //0x20
    union
    {
        uint8_t InProgressFlags;                                              //0x28
        struct
        {
            uint8_t KernelApcInProgress : 1;                                    //0x28
            uint8_t SpecialApcInProgress : 1;                                   //0x28
        };
    };
    uint8_t KernelApcPending;                                                 //0x29
    union
    {
        uint8_t UserApcPendingAll;                                            //0x2a
        struct
        {
            uint8_t SpecialUserApcPending : 1;                                  //0x2a
            uint8_t UserApcPending : 1;                                         //0x2a
        };
    };
};



//0x1 bytes (sizeof)
union _KWAIT_STATUS_REGISTER
{
    uint8_t Flags;                                                            //0x0
    uint8_t State : 3;                                                          //0x0
    uint8_t Affinity : 1;                                                       //0x0
    uint8_t Priority : 1;                                                       //0x0
    uint8_t Apc : 1;                                                            //0x0
    uint8_t UserApc : 1;                                                        //0x0
    uint8_t Alert : 1;                                                          //0x0
};



//0x8 bytes (sizeof)
union _KERNEL_SHADOW_STACK_LIMIT
{
    uintmax_t AllFields;                                                    //0x0
    uintmax_t ShadowStackType : 3;                                            //0x0
    uintmax_t Unused : 9;                                                     //0x0
    uintmax_t ShadowStackLimitPfn : 52;                                       //0x0
};

//0x1 bytes (sizeof)
struct _PS_PROTECTION
{
    union
    {
        uint8_t Level;                                                        //0x0
        struct
        {
            uint8_t Type : 3;                                                   //0x0
            uint8_t Audit : 1;                                                  //0x0
            uint8_t Signer : 4;                                                 //0x0
        };
    };
};




//0x8 bytes (sizeof)
struct _EX_FAST_REF
{
    union
    {
        void* Object;                                                       //0x0
        uintmax_t RefCnt : 4;                                                 //0x0
        uintmax_t Value;                                                    //0x0
    };
};



//0x8 bytes (sizeof)
struct _EX_PUSH_LOCK
{
    union
    {
        uintmax_t Value;    									              //0x0
        struct
        {
            uintmax_t Locked : 1;                                             //0x0
            uintmax_t Waiting : 1;                                            //0x0
            uintmax_t Waking : 1;                                             //0x0
            uintmax_t MultipleShared : 1;                                     //0x0
            uintmax_t Shared : 60;                                            //0x0
        };
        void* Ptr;                                                          //0x0
    };
};





//0x20 bytes (sizeof)
struct _ALPC_PROCESS_CONTEXT
{
    struct _EX_PUSH_LOCK Lock;                                              //0x0
    struct _LIST_ENTRY ViewListHead;                                        //0x8
    volatile uintmax_t PagedPoolQuotaCache;                                 //0x18
};



//0x8 bytes (sizeof)
struct _RTL_AVL_TREE
{
    struct _RTL_BALANCED_NODE* Root;                                        //0x0
};




//0x80 bytes (sizeof)
struct _MMSUPPORT_SHARED
{
    volatile long WorkingSetLock;                                           //0x0
    long GoodCitizenWaiting;                                                //0x4
    uintmax_t ReleasedCommitDebt;                                           //0x8
    uintmax_t ResetPagesRepurposedCount;                                    //0x10
    void* WsSwapSupport;                                                    //0x18
    void* CommitReleaseContext;                                             //0x20
    void* AccessLog;                                                        //0x28
    volatile uintmax_t ChargedWslePages;                                    //0x30
    uintmax_t ActualWslePages;                                              //0x38
    volatile long WorkingSetCoreLock;                                       //0x40
    void* ShadowMapping;                                                    //0x48
};



//0x4 bytes (sizeof)
struct _MMSUPPORT_FLAGS
{
    union
    {
        struct
        {
            uint8_t WorkingSetType : 3;                                         //0x0
            uint8_t Reserved0 : 3;                                              //0x0
            uint8_t MaximumWorkingSetHard : 1;                                  //0x0
            uint8_t MinimumWorkingSetHard : 1;                                  //0x0
            uint8_t SessionMaster : 1;                                          //0x1
            uint8_t TrimmerState : 2;                                           //0x1
            uint8_t Reserved : 1;                                               //0x1
            uint8_t PageStealers : 4;                                           //0x1
        };
        uint16_t u1;                                                          //0x0
    };
    uint8_t MemoryPriority;                                                   //0x2
    union
    {
        struct
        {
            uint8_t WsleDeleted : 1;                                            //0x3
            uint8_t SvmEnabled : 1;                                             //0x3
            uint8_t ForceAge : 1;                                               //0x3
            uint8_t ForceTrim : 1;                                              //0x3
            uint8_t NewMaximum : 1;                                             //0x3
            uint8_t CommitReleaseState : 2;                                     //0x3
        };
        uint8_t u2;                                                           //0x3
    };
};


//0xc0 bytes (sizeof)
struct _MMSUPPORT_INSTANCE
{
    ulong_t NextPageColor;                                                    //0x0
    volatile ulong_t PageFaultCount;                                          //0x4
    uintmax_t TrimmedPageCount;                                             //0x8
    struct _MMWSL_INSTANCE* VmWorkingSetList;                               //0x10
    struct _LIST_ENTRY WorkingSetExpansionLinks;                            //0x18
    volatile uintmax_t AgeDistribution[8];                                  //0x28
    struct _KGATE* ExitOutswapGate;                                         //0x68
    uintmax_t MinimumWorkingSetSize;                                        //0x70
    uintmax_t MaximumWorkingSetSize;                                        //0x78
    volatile uintmax_t WorkingSetLeafSize;                                  //0x80
    volatile uintmax_t WorkingSetLeafPrivateSize;                           //0x88
    volatile uintmax_t WorkingSetSize;                                      //0x90
    volatile uintmax_t WorkingSetPrivateSize;                               //0x98
    uintmax_t PeakWorkingSetSize;                                           //0xa0
    ulong_t HardFaultCount;                                                   //0xa8
    uint16_t LastTrimStamp;                                                   //0xac
    uint16_t PartitionId;                                                     //0xae
    uintmax_t SelfmapLock;                                                  //0xb0
    struct _MMSUPPORT_FLAGS Flags;                                          //0xb8
    long InterlockedFlags;                                                  //0xbc
};





//0x140 bytes (sizeof)
struct _MMSUPPORT_FULL
{
    struct _MMSUPPORT_INSTANCE Instance;                                    //0x0
    struct _MMSUPPORT_SHARED Shared;                                        //0xc0
};





//0x8 bytes (sizeof)
struct _MMPTE_HARDWARE
{
    uintmax_t Valid : 1;                                                      //0x0
    uintmax_t Dirty1 : 1;                                                     //0x0
    uintmax_t Owner : 1;                                                      //0x0
    uintmax_t WriteThrough : 1;                                               //0x0
    uintmax_t CacheDisable : 1;                                               //0x0
    uintmax_t Accessed : 1;                                                   //0x0
    uintmax_t Dirty : 1;                                                      //0x0
    uintmax_t LargePage : 1;                                                  //0x0
    uintmax_t Global : 1;                                                     //0x0
    uintmax_t CopyOnWrite : 1;                                                //0x0
    uintmax_t Unused : 1;                                                     //0x0
    uintmax_t Write : 1;                                                      //0x0
    uintmax_t PageFrameNumber : 40;                                           //0x0
    uintmax_t ReservedForSoftware : 4;                                        //0x0
    uintmax_t WsleAge : 4;                                                    //0x0
    uintmax_t WsleProtection : 3;                                             //0x0
    uintmax_t NoExecute : 1;                                                  //0x0
};



//0x8 bytes (sizeof)
struct _MMPTE_PROTOTYPE
{
    uintmax_t Valid : 1;                                                      //0x0
    uintmax_t DemandFillProto : 1;                                            //0x0
    uintmax_t HiberVerifyConverted : 1;                                       //0x0
    uintmax_t ReadOnly : 1;                                                   //0x0
    uintmax_t SwizzleBit : 1;                                                 //0x0
    uintmax_t Protection : 5;                                                 //0x0
    uintmax_t Prototype : 1;                                                  //0x0
    uintmax_t Combined : 1;                                                   //0x0
    uintmax_t Unused1 : 4;                                                    //0x0
    intmax_t ProtoAddress : 48;                                               //0x0
};



//0x8 bytes (sizeof)
struct _MMPTE_SOFTWARE
{
    uintmax_t Valid : 1;                                                      //0x0
    uintmax_t PageFileReserved : 1;                                           //0x0
    uintmax_t PageFileAllocated : 1;                                          //0x0
    uintmax_t ColdPage : 1;                                                   //0x0
    uintmax_t SwizzleBit : 1;                                                 //0x0
    uintmax_t Protection : 5;                                                 //0x0
    uintmax_t Prototype : 1;                                                  //0x0
    uintmax_t Transition : 1;                                                 //0x0
    uintmax_t PageFileLow : 4;                                                //0x0
    uintmax_t UsedPageTableEntries : 10;                                      //0x0
    uintmax_t ShadowStack : 1;                                                //0x0
    uintmax_t OnStandbyLookaside : 1;                                         //0x0
    uintmax_t Unused : 4;                                                     //0x0
    uintmax_t PageFileHigh : 32;                                              //0x0
};



//0x8 bytes (sizeof)
struct _MMPTE_TIMESTAMP
{
    uintmax_t MustBeZero : 1;                                                 //0x0
    uintmax_t Unused : 3;                                                     //0x0
    uintmax_t SwizzleBit : 1;                                                 //0x0
    uintmax_t Protection : 5;                                                 //0x0
    uintmax_t Prototype : 1;                                                  //0x0
    uintmax_t Transition : 1;                                                 //0x0
    uintmax_t PageFileLow : 4;                                                //0x0
    uintmax_t Reserved : 16;                                                  //0x0
    uintmax_t GlobalTimeStamp : 32;                                           //0x0
};



//0x8 bytes (sizeof)
struct _MMPTE_TRANSITION
{
    uintmax_t Valid : 1;                                                      //0x0
    uintmax_t Write : 1;                                                      //0x0
    uintmax_t OnStandbyLookaside : 1;                                         //0x0
    uintmax_t IoTracker : 1;                                                  //0x0
    uintmax_t SwizzleBit : 1;                                                 //0x0
    uintmax_t Protection : 5;                                                 //0x0
    uintmax_t Prototype : 1;                                                  //0x0
    uintmax_t Transition : 1;                                                 //0x0
    uintmax_t PageFrameNumber : 40;                                           //0x0
    uintmax_t Unused : 12;                                                    //0x0
};



//0x8 bytes (sizeof)
struct _MMPTE_SUBSECTION
{
    uintmax_t Valid : 1;                                                      //0x0
    uintmax_t Unused0 : 2;                                                    //0x0
    uintmax_t OnStandbyLookaside : 1;                                         //0x0
    uintmax_t SwizzleBit : 1;                                                 //0x0
    uintmax_t Protection : 5;                                                 //0x0
    uintmax_t Prototype : 1;                                                  //0x0
    uintmax_t ColdPage : 1;                                                   //0x0
    uintmax_t Unused2 : 3;                                                    //0x0
    uintmax_t ExecutePrivilege : 1;                                           //0x0
    intmax_t SubsectionAddress : 48;                                          //0x0
};




//0x8 bytes (sizeof)
struct _MMPTE_LIST
{
    uintmax_t Valid : 1;                                                      //0x0
    uintmax_t OneEntry : 1;                                                   //0x0
    uintmax_t filler0 : 2;                                                    //0x0
    uintmax_t SwizzleBit : 1;                                                 //0x0
    uintmax_t Protection : 5;                                                 //0x0
    uintmax_t Prototype : 1;                                                  //0x0
    uintmax_t Transition : 1;                                                 //0x0
    uintmax_t filler1 : 16;                                                   //0x0
    uintmax_t NextEntry : 36;                                                 //0x0
};



//0x8 bytes (sizeof)
struct _MMPTE
{
    union
    {
        uintmax_t Long;                                                     //0x0
        volatile uintmax_t VolatileLong;                                    //0x0
        struct _MMPTE_HARDWARE Hard;                                        //0x0
        struct _MMPTE_PROTOTYPE Proto;                                      //0x0
        struct _MMPTE_SOFTWARE Soft;                                        //0x0
        struct _MMPTE_TIMESTAMP TimeStamp;                                  //0x0
        struct _MMPTE_TRANSITION Trans;                                     //0x0
        struct _MMPTE_SUBSECTION Subsect;                                   //0x0
        struct _MMPTE_LIST List;                                            //0x0
    } u;                                                                    //0x0
};




#endif //!UNDEFINEDSTRUCTS_HPP
