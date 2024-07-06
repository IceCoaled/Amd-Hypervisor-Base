#ifndef CUSTOMFUNCTIONS_HPP
#define CUSTOMFUNCTIONS_HPP


#include "cstdint"

#include <ntddk.h>


using SYSTEM_MODULE_ENTRY = struct _SYSTEM_MODULE_ENTRY
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[256];
};


using SYSTEM_INFORMATION_CLASS = enum class _SYSTEM_INFORMATION_CLASS
{
	SystemModuleInformation = 11
};

using SYSTEM_MODULE_INFORMATION = struct _SYSTEM_MODULE_INFORMATION
{
	ULONG count;
	SYSTEM_MODULE_ENTRY module[1];
};



EXTERN_C NTSTATUS __stdcall ZwQuerySystemInformation(
	_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_   PVOID                    SystemInformation,
	_In_      ULONG                    SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength
);



namespace Inlines
{

	static UCHAR _MIGETPTEADDRESS[] = { 0x48, 0xC1, 0xE9, 0x09, 0x48, 0xB8, 0xF8, 0xFF, 0xFF, 0xFF, 0x7f, 0x00, 0x00, 0x00, 0x48, 0x23, 0xC8, 0x48, 0xB8 };
	static void* miGetPteAddress = nullptr;
	static SYSTEM_MODULE_INFORMATION* moduleList = nullptr;
	static void* kernelBase = nullptr;
	static size_t szKernel = 0;


	template <typename T>
	size_t strLen(__in T string);

	uintmax_t CharToInt(__inout char* string);

	void lowerCase(__in char* string, __in size_t len);

	int memCpr(__in void* lhs, __in void* rhs, __in size_t length);

	void memSet(__in void* mem, __in uint8_t setValue, __in size_t length);

	void memCpy(__in void* dest, __in void* src, __in size_t length);

	template <typename T>
	int strCmp(__in T string1, __in T string2);

	void* getVirtualPte(__in uintmax_t systemAddr);
	
	void* getPhysicalPte(__in uintmax_t physicalAddr);

	void* poolAlloc(__in size_t length);

	void poolFree(__in void* buffer);

	void* contigAlloc(__in size_t length);

	void contigFree(__in void* buffer, __in size_t szBuffer);

	void* signatureScanner(__in UCHAR* memory, __in size_t length, __in UCHAR* pattern, __in size_t patternLength);

	NTSTATUS GetModuleList(__inout SYSTEM_MODULE_INFORMATION** modulelist);

	void* FindModule(__inout SYSTEM_MODULE_INFORMATION** modulelist, __inout size_t* szModule, const char* moduleToFind);
	
}; // namespace Inlines

#endif // !CUSTOMFUNCTIONS_HXX