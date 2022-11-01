#include "public.h"

/*

win11 22H2
if mapper need 2.step
1.HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\CI\Config\VulnerableDriverBlocklistEnable
2.自带安全中心,内核隔离关闭

*/


#ifdef __cplusplus  
extern "C" {
#endif

	KoMCallBack g_KoMCallBack;

	NTSTATUS UnloadDriver(PDRIVER_OBJECT derver);
	NTSTATUS DispatchIOCTL(PDEVICE_OBJECT device, PIRP pirp);

	PDRIVER_OBJECT g_DriverObject;




	NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING path)
	{
		NTSTATUS status = STATUS_SUCCESS;
		g_DriverObject = driver;
		KdPrint(("path：%wZ", path));

		driver->DriverUnload = (PDRIVER_UNLOAD)UnloadDriver;

		auto defaultCreateCloseFunction = [](PDEVICE_OBJECT device, PIRP pirp) {
			pirp->IoStatus.Status = STATUS_SUCCESS;
			IoCompleteRequest(pirp, IO_NO_INCREMENT);
			return STATUS_SUCCESS;
		};

		driver->MajorFunction[IRP_MJ_CREATE] = defaultCreateCloseFunction;
		driver->MajorFunction[IRP_MJ_CLOSE] = defaultCreateCloseFunction;

		driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIOCTL;

		UNICODE_STRING devname;
		RtlInitUnicodeString(&devname, NAME);
		PDEVICE_OBJECT pdev = NULL;
		status = IoCreateDevice(driver, sizeof(driver->DriverExtension),
			&devname, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, TRUE, &pdev);

		pdev->Flags |= DO_DIRECT_IO;
		pdev->AlignmentRequirement = FILE_WORD_ALIGNMENT;

		UNICODE_STRING devlink;
		RtlInitUnicodeString(&devlink, LINKNAME);
		status = IoCreateSymbolicLink(&devlink, &devname);


		SearchKdbServiceCallBack();
		SearchMouServiceCallBack();
		pdev->Flags &= ~DO_DEVICE_INITIALIZING;
		
		

		return status;
	}


	NTSTATUS UnloadDriver(PDRIVER_OBJECT derver)
	{
		DbgPrint("bye!\n");
		UNICODE_STRING devlink;
		RtlInitUnicodeString(&devlink, LINKNAME);
		IoDeleteSymbolicLink(&devlink);
		IoDeleteDevice(derver->DeviceObject);
		return STATUS_SUCCESS;
	}


	NTSTATUS DispatchIOCTL(PDEVICE_OBJECT device, PIRP pirp)
	{
		NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

		PIO_STACK_LOCATION pirpstack = IoGetCurrentIrpStackLocation(pirp);

		ULONG ioctlcode = pirpstack->Parameters.DeviceIoControl.IoControlCode;

		PVOID bufferPtr = pirp->AssociatedIrp.SystemBuffer;
		ULONG inSize = pirpstack->Parameters.DeviceIoControl.InputBufferLength;
		ULONG outSize = pirpstack->Parameters.DeviceIoControl.OutputBufferLength;

		PKEYBOARD_INPUT_DATA KbdInputDataStart, KbdInputDataEnd;
		PMOUSE_INPUT_DATA 	 MouseInputDataStart, MouseInputDataEnd;
		ULONG 				 InputDataConsumed;
		MOUSE_INPUT_DATA mid;
		KEYBOARD_INPUT_DATA kid;


		switch (ioctlcode)
		{
			case CTLMOUSE:
			{
				mid = *(PMOUSE_INPUT_DATA)bufferPtr;

				MouseInputDataStart = &mid;
				MouseInputDataEnd = MouseInputDataStart + 1;

				g_KoMCallBack.MouseClassServiceCallback(g_KoMCallBack.MouDeviceObject,
					MouseInputDataStart,
					MouseInputDataEnd,
					&InputDataConsumed);

				pirp->IoStatus.Information = outSize;
				break;
			}
			case CTLKEYBORD:
			{
				kid = *(PKEYBOARD_INPUT_DATA)bufferPtr;
				KbdInputDataStart = &kid;
				KbdInputDataEnd = KbdInputDataStart + 1;

				g_KoMCallBack.KeyboardClassServiceCallback(g_KoMCallBack.KdbDeviceObject,
					KbdInputDataStart,
					KbdInputDataEnd,
					&InputDataConsumed);

				pirp->IoStatus.Information = outSize;
				break;
			}

			default:
			{
				break;
			}
		}

		pirp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(pirp, IO_NO_INCREMENT);
		return STATUS_SUCCESS;
	}




	NTSTATUS DriverEntry2(PDRIVER_OBJECT driver, PUNICODE_STRING path)
	{
		#define FILE_DEVICE_MIRRORE 0x3138

		UNICODE_STRING devname;
		RtlInitUnicodeString(&devname, NAME);
		PDEVICE_OBJECT pdev = NULL;
		IoCreateDevice(driver, 0,
			&devname, FILE_DEVICE_MIRRORE, FILE_DEVICE_SECURE_OPEN, FALSE, &pdev);
		
		UNICODE_STRING devlink;
		RtlInitUnicodeString(&devlink, L"\\??\\keysimulationdriverlink");
		IoCreateSymbolicLink(&devlink, &devname);

		SetFlag(driver->Flags, DO_DIRECT_IO);

		auto defaultNotSupport = [](PDEVICE_OBJECT device_obj, PIRP irp) {
			UNREFERENCED_PARAMETER(device_obj);
			irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
			IofCompleteRequest(irp, IO_NO_INCREMENT);
			return irp->IoStatus.Status;

		};
		for (int t = 0; t <= IRP_MJ_MAXIMUM_FUNCTION; t++)
			driver->MajorFunction[t] = defaultNotSupport;


		auto defaultCreateCloseFunction = [](PDEVICE_OBJECT device, PIRP pirp) {
			pirp->IoStatus.Status = STATUS_SUCCESS;
			IoCompleteRequest(pirp, IO_NO_INCREMENT);
			return STATUS_SUCCESS;
		};

		driver->MajorFunction[IRP_MJ_CREATE] = defaultCreateCloseFunction;
		driver->MajorFunction[IRP_MJ_CLOSE] = defaultCreateCloseFunction;
		driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIOCTL;
		driver->DriverUnload = NULL;
		
		pdev->AlignmentRequirement = FILE_WORD_ALIGNMENT;

		SearchKdbServiceCallBack();
		SearchMouServiceCallBack();
		ClearFlag(pdev->Flags, DO_DEVICE_INITIALIZING);


		return STATUS_SUCCESS;
	}

	// for mapper diy entry   [GsDriverEntry]
	NTSTATUS custom_entry_point(PVOID a1, PVOID a2)
	{
		UNICODE_STRING driName;
		RtlInitUnicodeString(&driName, L"\\Driver\\keysimulationdriver");
		return IoCreateDriver(&driName, &DriverEntry2);
	}



#ifdef __cplusplus
}
#endif