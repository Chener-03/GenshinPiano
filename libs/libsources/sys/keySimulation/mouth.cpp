#include "mouth.h"

#ifdef __cplusplus  
extern "C" {
#endif


	NTSTATUS SearchServiceFromMouExt(PDRIVER_OBJECT MouDriverObject, PDEVICE_OBJECT pPortDev)
	{
		PDEVICE_OBJECT pTargetDeviceObject = NULL;
		UCHAR* DeviceExt;
		int i = 0;
		NTSTATUS status;
		PVOID KbdDriverStart;
		ULONG KbdDriverSize = 0;
		PDEVICE_OBJECT  pTmpDev;
		UNICODE_STRING  kbdDriName;

		KbdDriverStart = MouDriverObject->DriverStart;
		KbdDriverSize = MouDriverObject->DriverSize;

		status = STATUS_UNSUCCESSFUL;

		RtlInitUnicodeString(&kbdDriName, L"\\Driver\\mouclass");
		pTmpDev = pPortDev;
		while (pTmpDev->AttachedDevice != NULL)
		{
			KdPrint(("Att:  0x%x", pTmpDev->AttachedDevice));
			KdPrint(("Dri Name : %wZ", &pTmpDev->AttachedDevice->DriverObject->DriverName));
			if (RtlCompareUnicodeString(&pTmpDev->AttachedDevice->DriverObject->DriverName,
				&kbdDriName, TRUE) == 0)
			{
				KdPrint(("Find Object Device: "));
				break;
			}
			pTmpDev = pTmpDev->AttachedDevice;
		}
		if (pTmpDev->AttachedDevice == NULL)
		{
			return status;
		}
		pTargetDeviceObject = MouDriverObject->DeviceObject;
		while (pTargetDeviceObject)
		{
			if (pTmpDev->AttachedDevice != pTargetDeviceObject)
			{
				pTargetDeviceObject = pTargetDeviceObject->NextDevice;
				continue;
			}
			DeviceExt = (UCHAR*)pTmpDev->DeviceExtension;
			g_KoMCallBack.MouDeviceObject = NULL;
			  
			for (i = 0; i < 4096; i++, DeviceExt++)
			{
				PVOID tmp;
				if (!MmIsAddressValid(DeviceExt))
				{
					break;
				}
				  
				if (g_KoMCallBack.MouDeviceObject && g_KoMCallBack.MouseClassServiceCallback)
				{
					status = STATUS_SUCCESS;
					break;
				}
				 
				tmp = *(PVOID*)DeviceExt;
				if (tmp == pTargetDeviceObject)
				{
					g_KoMCallBack.MouDeviceObject = pTargetDeviceObject;
					continue;
				}

				 
				if ((tmp > KbdDriverStart) && (tmp < (UCHAR*)KbdDriverStart + KbdDriverSize) &&
					(MmIsAddressValid(tmp)))
				{
					 
					g_KoMCallBack.MouseClassServiceCallback = (MY_MOUSECALLBACK)tmp;
					 
					status = STATUS_SUCCESS;
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


	NTSTATUS SearchMouServiceCallBack()
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

		// todo: open usb mouth driver obj  
		RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\mouhid");
		status = ObReferenceObjectByName(&uniNtNameString,
			OBJ_CASE_INSENSITIVE, NULL, 0,
			*IoDriverObjectType,
			KernelMode,
			NULL,
			(PVOID*)&KbdhidDriverObject);
		if (!NT_SUCCESS(status))
		{
			KdPrint(("δ�ҵ�usb�����������\n"));
		}
		else
		{
			ObDereferenceObject(KbdhidDriverObject);
			KdPrint(("�ҵ�usb�����������\n"));
		}
		// todo: open ps/2 mouth driver obj  
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
			KdPrint(("δ�ҵ�ps/2����������� \n"));
		}
		else
		{
			ObDereferenceObject(Kbd8042DriverObject);
			KdPrint(("�ҵ�ps/2�����������\n"));
		}
		// todo: None found
		if (!Kbd8042DriverObject && !KbdhidDriverObject)
		{
			KdPrint(("�������������δ�ҵ� �޷��������ģ��\n"));
			return STATUS_SUCCESS;
		}
		// todo: All found, using USB
		if (KbdhidDriverObject)
		{
			UsingDriverObject = KbdhidDriverObject;
		}
		else
		{
			UsingDriverObject = Kbd8042DriverObject;
		}
		RtlInitUnicodeString(&uniNtNameString, L"\\Driver\\mouclass");
		status = ObReferenceObjectByName(&uniNtNameString,
			OBJ_CASE_INSENSITIVE, NULL,
			0,
			*IoDriverObjectType,
			KernelMode,
			NULL,
			(PVOID*)&KbdDriverObject);
		if (!NT_SUCCESS(status))
		{
			// todo: false
			KdPrint(("Attach: Coundn't get the Mouse driver Object\n"));
			return STATUS_UNSUCCESSFUL;
		}
		else
		{
			ObDereferenceObject(KbdDriverObject);
		}
		// todo: find KbdDriverObject device object
		UsingDeviceObject = UsingDriverObject->DeviceObject;
		while (UsingDeviceObject)
		{
			status = SearchServiceFromMouExt(KbdDriverObject, UsingDeviceObject);
			if (status == STATUS_SUCCESS)
			{
				break;
			}
			UsingDeviceObject = UsingDeviceObject->NextDevice;
		}
		if (g_KoMCallBack.MouDeviceObject && g_KoMCallBack.MouseClassServiceCallback)
		{
			KdPrint(("Find MouseClassServiceCallback\n"));
		}
		return status;
	}





#ifdef __cplusplus
}
#endif


