#pragma once

/**
 * @author chen
 * @email  chen@chener.xyz
 */

#include <ntifs.h>
#include <ntddk.h>
#include <windef.h>
#include <ntddstor.h>
#include <mountdev.h>
#include <ntddvol.h>
#include <ntimage.h>

#include "mouth.h"
#include "kbd.h"

#define NAME			L"\\Device\\keysimulationdriver"
#define LINKNAME		L"\\??\\keysimulationdriverlink"

#define CTLMOUSE		CTL_CODE(FILE_DEVICE_UNKNOWN,0x832, METHOD_BUFFERED, FILE_ALL_ACCESS) //mouth
#define CTLKEYBORD		CTL_CODE(FILE_DEVICE_UNKNOWN,0x833, METHOD_BUFFERED, FILE_ALL_ACCESS) //keybord
 


#ifdef __cplusplus  
extern "C" {
#endif



typedef struct _KEYBOARD_INPUT_DATA
{
	//keybord
	USHORT		UnitId;
	USHORT		MakeCode;
	USHORT		Flags;
	USHORT		Reserved;
	ULONG		ExtraInformation;
} KEYBOARD_INPUT_DATA, * PKEYBOARD_INPUT_DATA;

typedef struct _MOUSE_INPUT_DATA
{
	//mouth
	USHORT			UnitId;
	USHORT			Flags;
	union
	{
		ULONG		Buttons;
		struct
		{
			USHORT	ButtonFlags;
			USHORT  ButtonData;
		};
	};
	ULONG			RawButtons;
	LONG			LastX;
	LONG			LastY;
	ULONG			ExtraInformation;
} MOUSE_INPUT_DATA, * PMOUSE_INPUT_DATA;




typedef VOID(*MY_MOUSECALLBACK) (PDEVICE_OBJECT  DeviceObject, PMOUSE_INPUT_DATA  InputDataStart, PMOUSE_INPUT_DATA  InputDataEnd, PULONG  InputDataConsumed);
typedef VOID(*MY_KEYBOARDCALLBACK) (PDEVICE_OBJECT  DeviceObject, PKEYBOARD_INPUT_DATA  InputDataStart, PKEYBOARD_INPUT_DATA  InputDataEnd, PULONG  InputDataConsumed);




NTKERNELAPI NTSTATUS ObReferenceObjectByName(
	IN PUNICODE_STRING ObjectName,
	IN ULONG Attributes,
	IN PACCESS_STATE PassedAccessState OPTIONAL,
	IN ACCESS_MASK DesiredAccess OPTIONAL,
	IN POBJECT_TYPE ObjectType,
	IN KPROCESSOR_MODE AccessMode,
	IN OUT PVOID ParseContext OPTIONAL,
	OUT PVOID* Object
);

extern POBJECT_TYPE* IoDriverObjectType;


typedef struct _KoMCallBack
{
	PDEVICE_OBJECT			KdbDeviceObject;
	MY_KEYBOARDCALLBACK		KeyboardClassServiceCallback;
	PDEVICE_OBJECT			MouDeviceObject;
	MY_MOUSECALLBACK		MouseClassServiceCallback;
}KoMCallBack, * pKoMCallBack;


extern KoMCallBack g_KoMCallBack;



typedef struct _SYSTEM_MODULE
{
	ULONG_PTR Reserved[2];
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
} SYSTEM_MODULE, * PSYSTEM_MODULE;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG_PTR ulModuleCount;
	SYSTEM_MODULE Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

NTKERNELAPI PVOID NTAPI RtlFindExportedRoutineByName(_In_ PVOID ImageBase, _In_ PCCH RoutineName);

NTSTATUS
NTAPI
ZwQuerySystemInformation(
	DWORD32 systemInformationClass,
	PVOID systemInformation,
	ULONG systemInformationLength,
	PULONG returnLength);




#ifdef __cplusplus
}
#endif