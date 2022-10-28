#include "kbd.h"



#ifdef __cplusplus  
extern "C" {
#endif

	NTSTATUS SearchServiceFromKdbExt(PDRIVER_OBJECT KbdDriverObject, PDEVICE_OBJECT pPortDev)
	{
		PDEVICE_OBJECT pTargetDeviceObject = NULL;
		UCHAR* DeviceExt;
		int i = 0;
		NTSTATUS status;
		PVOID KbdDriverStart;
		ULONG KbdDriverSize = 0;
		PDEVICE_OBJECT  pTmpDev;
		UNICODE_STRING  kbdDriName;

		KbdDriverStart = KbdDriverObject->DriverStart;
		KbdDriverSize = KbdDriverObject->DriverSize;

		status = STATUS_UNSUCCESSFUL;

		RtlInitUnicodeString(&kbdDriName, L"\\Driver\\kbdclass");
		pTmpDev = pPortDev;
		while (pTmpDev->AttachedDevice != NULL)
		{
			KdPrint(("Att:  0x%x", pTmpDev->AttachedDevice));
			KdPrint(("Dri Name : %wZ", &pTmpDev->AttachedDevice->DriverObject->DriverName));
			if (RtlCompareUnicodeString(&pTmpDev->AttachedDevice->DriverObject->DriverName,
				&kbdDriName, TRUE) == 0)
			{
				break;
			}
			pTmpDev = pTmpDev->AttachedDevice;
		}
		if (pTmpDev->AttachedDevice == NULL)
		{
			return status;
		}

		pTargetDeviceObject = KbdDriverObject->DeviceObject;
		while (pTargetDeviceObject)
		{
			if (pTmpDev->AttachedDevice != pTargetDeviceObject)
			{
				pTargetDeviceObject = pTargetDeviceObject->NextDevice;
				continue;
			}
			DeviceExt = (UCHAR*)pTmpDev->DeviceExtension;
			g_KoMCallBack.KdbDeviceObject = NULL;
			   
			for (i = 0; i < 4096; i++, DeviceExt++)
			{
				PVOID tmp;
				if (!MmIsAddressValid(DeviceExt))
				{
					break;
				}
				   
				if (g_KoMCallBack.KdbDeviceObject && g_KoMCallBack.KeyboardClassServiceCallback)
				{
					status = STATUS_SUCCESS;
					break;
				}
				 
				tmp = *(PVOID*)DeviceExt;
				if (tmp == pTargetDeviceObject)
				{
					g_KoMCallBack.KdbDeviceObject = pTargetDeviceObject;
					continue;
				}

				 
				if ((tmp > KbdDriverStart) && (tmp < (UCHAR*)KbdDriverStart + KbdDriverSize) &&
					(MmIsAddressValid(tmp)))
				{
					 
					g_KoMCallBack.KeyboardClassServiceCallback = (MY_KEYBOARDCALLBACK)tmp;
				}
			}
			if (status == STATUS_SUCCESS)
			{
				break;
			}
			 
			pTargetDeviceObject = pTargetDeviceObject->NextDevice;
		}
		return status;
	}



	NTSTATUS SearchKdbServiceCallBack()
	{
		NTSTATUS status = STATUS_SUCCESS;
		UNICODE_STRING uniNtNameString;
		PDEVICE_OBJECT pTargetDeviceObject = NULL;
		PDRIVER_OBJECT KbdDriverObject = NULL;
		PDRIVER_OBJECT KbdhidDriverObject = NULL;
		PDRIVER_OBJECT Kbd8042DriverObject = NULL;
		PDRIVER_OBJECT UsingDriverObject = NULL;
		PDEVICE_OBJECT UsingDeviceObject = NULL;

		PVOID UsingDeviceExt = NULL;

 
		RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\kbdhid");

		status = ObReferenceObjectByName(&uniNtNameString,
			OBJ_CASE_INSENSITIVE, NULL, 0,
			*IoDriverObjectType,
			KernelMode,
			NULL,
			(PVOID*)&KbdhidDriverObject);

		if (!NT_SUCCESS(status))
		{
			KdPrint(("Couldn't get the USB driver Object\n"));
		}
		else
		{
			ObDereferenceObject(KbdhidDriverObject);
			KdPrint(("get the USB driver Object\n"));
		}
   
		RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\i8042prt");
		status = ObReferenceObjectByName(&uniNtNameString,
			OBJ_CASE_INSENSITIVE,
			NULL, 0,
			*IoDriverObjectType,
			KernelMode,
			NULL,
			(PVOID*)&Kbd8042DriverObject);
		if (!NT_SUCCESS(status))
		{
			KdPrint(("Couldn't get the PS/2 driver Object %08x\n", status));
		}
		else
		{
			ObDereferenceObject(Kbd8042DriverObject);
			KdPrint(("get the PS/2 driver Object\n"));
		}
	 
		if (!Kbd8042DriverObject && !KbdhidDriverObject)
		{
			return STATUS_SUCCESS;
		}
	 
		UsingDriverObject = Kbd8042DriverObject ? Kbd8042DriverObject : KbdhidDriverObject;

		RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\kbdclass");
		status = ObReferenceObjectByName(&uniNtNameString,
			OBJ_CASE_INSENSITIVE, NULL,
			0,
			*IoDriverObjectType,
			KernelMode,
			NULL,
			(PVOID*)&KbdDriverObject);
		if (!NT_SUCCESS(status))
		{
		 
			KdPrint(("MyAttach: Coundn't get the kbd driver Object\n"));
			return STATUS_UNSUCCESSFUL;
		}
		else
		{
			ObDereferenceObject(KbdDriverObject);
		}

 
		UsingDeviceObject = UsingDriverObject->DeviceObject;
		while (UsingDeviceObject)
		{
			status = SearchServiceFromKdbExt(KbdDriverObject, UsingDeviceObject);
			if (status == STATUS_SUCCESS)
			{
				break;
			}
			UsingDeviceObject = UsingDeviceObject->NextDevice;
		}

	 
		if (g_KoMCallBack.KdbDeviceObject && g_KoMCallBack.KeyboardClassServiceCallback)
		{
			KdPrint(("Find keyboradClassServiceCallback\n"));
		}

		return status;
	}





#ifdef __cplusplus
}
#endif

