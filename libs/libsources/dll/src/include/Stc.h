//
// Created by chen on 2022/10/11.
//

#ifndef GLOBLEHOTKEY4J_STC_H
#define GLOBLEHOTKEY4J_STC_H
#include <windows.h>
#include "jni.h"

typedef bool (*FUNCTION)(JNIEnv *,jmethodID,jobject);


#define NAME			L"\\Device\\keysimulationdriver"
#define LINKNAME		L"\\??\\keysimulationdriverlink"
#define R3NAME          L"keySimulation"
#define SYSFILENAME     L"keySimulation.sys"

#define CTLMOUSE		CTL_CODE(FILE_DEVICE_UNKNOWN,0x832, METHOD_BUFFERED, FILE_ALL_ACCESS) //mouth
#define CTLKEYBORD		CTL_CODE(FILE_DEVICE_UNKNOWN,0x833, METHOD_BUFFERED, FILE_ALL_ACCESS) //keybord

struct EventFunctionMapper{
    int event;
    HWND winHwnd;
    FUNCTION  function;
    JNIEnv * e;
    jmethodID method;
    jobject obj;
};


typedef struct _KEYBOARD_INPUT_DATA
{
    //键盘
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

#endif //GLOBLEHOTKEY4J_STC_H
