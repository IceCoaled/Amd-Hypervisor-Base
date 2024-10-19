#ifndef CUSTOMFUNCTIONS_HPP
#define CUSTOMFUNCTIONS_HPP


#include "cstdint"

#include <ntddk.h>

// Defines a structure representing a system module entry, used to store information about a loaded system module.
using SYSTEM_MODULE_ENTRY = struct _SYSTEM_MODULE_ENTRY
{
	HANDLE Section;                // Handle to the section object representing the module.
	PVOID MappedBase;              // Pointer to the base address where the module is mapped in memory.
	PVOID ImageBase;               // Pointer to the base address of the module image.
	ULONG ImageSize;               // Size of the module image in bytes.
	ULONG Flags;                   // Flags that provide information about the module (e.g., load status).
	USHORT LoadOrderIndex;         // Load order index of the module.
	USHORT InitOrderIndex;         // Initialization order index of the module.
	USHORT LoadCount;              // Count of how many times the module is loaded.
	USHORT OffsetToFileName;       // Offset to the file name in the FullPathName.
	UCHAR FullPathName[ 256 ];       // Full path name of the module.
};

// Defines an enumeration for the system information class.
// This is used when querying system information using ZwQuerySystemInformation.
using SYSTEM_INFORMATION_CLASS = enum class _SYSTEM_INFORMATION_CLASS
{
	SystemModuleInformation = 11   // Represents information about system modules.
};

// Defines a structure representing system module information.
// It contains a count of modules and an array of module entries.
using SYSTEM_MODULE_INFORMATION = struct _SYSTEM_MODULE_INFORMATION
{
	ULONG count;                   // Number of modules in the system.
	SYSTEM_MODULE_ENTRY module[ 1 ]; // Array of system module entries.
};

// Declaration of the ZwQuerySystemInformation function, which retrieves various kinds of system information.
// Parameters:
// - SystemInformationClass: Specifies the type of system information to be retrieved.
// - SystemInformation: Pointer to a buffer that receives the system information.
// - SystemInformationLength: Length of the SystemInformation buffer.
// - ReturnLength: Optional pointer that receives the size of the data returned.
EXTERN_C NTSTATUS __stdcall ZwQuerySystemInformation(
	_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_   PVOID                    SystemInformation,
	_In_      ULONG                    SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength
);



/**
 * @brief The Inlines namespace contains a collection of custom utility functions.
 *
 * Many functions within this namespace are inlined for performance reasons. These functions
 * provide a variety of operations such as string manipulation, memory management, signature
 * scanning, and module handling.
 *
 * @remarks
 * The functions in the Inlines namespace are designed for low-level system tasks and are
 * optimized for speed through inlining. These functions include, but are not limited to,
 * memory allocation, memory comparison, and address translation. The namespace also provides
 * functionality for working with system modules, such as retrieving and searching through
 * loaded module lists.
 *
 * Note that function comments are not provided in the headers, unless they are part of a struct or class.
 */
namespace Inlines
{

	static UCHAR _MIGETPTEADDRESS[] = { 0x48, 0xC1, 0xE9, 0x09, 0x48, 0xB8, 0xF8, 0xFF, 0xFF, 0xFF, 0x7f, 0x00, 0x00, 0x00, 0x48, 0x23, 0xC8, 0x48, 0xB8 };
	static void* miGetPteAddress = nullptr;
	static SYSTEM_MODULE_INFORMATION* moduleList = nullptr;
	static void* kernelBase = nullptr;
	static size_t szKernel = 0;


	template <typename T>
	size_t strLen(__inout T string);

	uintmax_t CharToInt(__inout char* string);

	void lowerCase(__in char* string, __in size_t len);

	int memCpr(__in void* lhs, __in void* rhs, __in size_t length);

	void memSet(__inout void* mem, __in const uint8_t setValue, __in size_t length);

	void memCpy(__inout void* dest, __inout void* src, __in size_t length);

	/// both must be of type asci or unicode, cant mix	
	template <typename S1, typename S2>
	int strCmp(__inout S1 string1, __inout S2 string2);

	void* getVirtualPte(__in uintmax_t systemAddr);
	
	void* getPhysicalPte(__in uintmax_t physicalAddr);

	void* poolAlloc(__in size_t length);

	void poolFree(__inout void* buffer);

	void* contigAlloc(__in size_t length);

	void contigFree(__inout void* buffer, __in size_t szBuffer);

	void* signatureScanner(__inout UCHAR* memory, __in size_t length, __inout UCHAR* pattern, __in size_t patternLength);

	NTSTATUS GetModuleList(__inout SYSTEM_MODULE_INFORMATION** modulelist);

	void* FindModule(__inout SYSTEM_MODULE_INFORMATION** modulelist, __inout size_t* szModule, const char* moduleToFind);
	
}; // namespace Inlines

#endif // !CUSTOMFUNCTIONS_HXX