#include "Hypervisor.hpp"
#include "CustomFunctions.hpp"



EXTERN_C DRIVER_INITIALIZE DriverEntry;


static PDEVICE_OBJECT pDeviceObject;
static UNICODE_STRING deviceName;
static UNICODE_STRING dosName;

#define drv_device L"\\Device\\Hypervisor"
#define drv_dos_device L"\\DosDevices\\Hypervisor"



static void UnloadDriver(PDRIVER_OBJECT pDriverObject)
{

    IoDeleteSymbolicLink(&dosName);
    IoDeleteDevice(pDriverObject->DeviceObject);

    ZwUnloadDriver(&deviceName);
}


NTSTATUS DriverEntry(__in PDRIVER_OBJECT  DriverObject, __in PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	KPRINT("[+]DriverEntry Called\n");
	
	NTSTATUS status;
	RtlInitUnicodeString(&deviceName, drv_device);
	RtlInitUnicodeString(&dosName, drv_dos_device);

	status = IoCreateDevice(DriverObject, 0, &deviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	status = IoCreateSymbolicLink(&dosName, &deviceName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDeviceObject);
		return status;
	}

	DriverObject->DriverUnload = UnloadDriver;

	Inlines::GetModuleList(&Inlines::moduleList);
	if (Inlines::moduleList->count == 0)
	{
		KPRINT("[-]Failed To Get Module List\n");
		Inlines::poolFree(&Inlines::moduleList);
		return STATUS_SUCCESS;
	}

	KPRINT("[+]Module List Success\n");

	Inlines::kernelBase = Inlines::FindModule(&Inlines::moduleList, &Inlines::szKernel, "ntoskrnl.exe");
	if (!Inlines::kernelBase)
	{
		KPRINT("[-]Failed To Find Kernel Base\n");
		Inlines::poolFree(&Inlines::moduleList);
		return STATUS_SUCCESS;
	}

	KPRINT("[+]Found kernel Base Loacated At : 0x%I64X\n", Inlines::kernelBase);

	Inlines::miGetPteAddress = Inlines::signatureScanner((UCHAR*)Inlines::kernelBase, Inlines::szKernel, Inlines::_MIGETPTEADDRESS, sizeof(Inlines::_MIGETPTEADDRESS));
	if (!Inlines::miGetPteAddress)
	{
		KPRINT("[-]Failed To Find MiGetPteAddress\n");
		Inlines::poolFree(&Inlines::moduleList);
		return STATUS_SUCCESS;
	}

	KPRINT("[+]Found MiGetPteAddress Located At : 0x%I64X\n", Inlines::miGetPteAddress);

	auto hypervisor = reinterpret_cast<Hypervisor*>(Inlines::contigAlloc(sizeof(Hypervisor)));
	if (!hypervisor)
	{
		KPRINT("[-]Failed to allocate hypervisor\n");
		Inlines::poolFree(&Inlines::moduleList);
		return STATUS_SUCCESS;
	}

	KPRINT("[+]Allocated Hypervisor At 0x%I64X\n", hypervisor);

	hypervisor->HypervisorInit();

	return STATUS_SUCCESS;
}

