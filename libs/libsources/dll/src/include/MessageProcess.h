//
// Created by chen on 2022/10/11.
//

#ifndef GLOBLEHOTKEY4J_MESSAGEPROCESS_H
#define GLOBLEHOTKEY4J_MESSAGEPROCESS_H
#pragma once
#include "Stc.h"
#include "list"
#include "MinHook.h"
#include <windows.h>
#include <mutex>
#include "jni.h"

typedef LRESULT (*RealDispatchMessageFun)(_In_ CONST MSG *lpMsg);
WINUSERAPI LRESULT WINAPI ProxyDispatchMessageW(_In_ CONST MSG *lpMsg);

static std::mutex mutex_MessageProcess;
class MessageProcess {
public:

    static MessageProcess* getInstance()
    {
        if (instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(mutex_MessageProcess);
            if(instance == nullptr)
            {
                instance = new MessageProcess();
            }
        }
        return  instance;
    }

    bool addEvent(int event, int hwnd ,FUNCTION callback,JNIEnv * e,jmethodID i,jobject obj);

    bool deleteEvent(int event);

    bool startHook();

    bool stopHook();

    std::list<EventFunctionMapper> ef;

    LPVOID realPtr = nullptr;

    int addHotKey(char* hotKeyName,int hwnd,int modifiers,int key);

    void deleteHotKey(int hwnd,int id);

private:
    static MessageProcess* instance;
    MessageProcess();
    ~MessageProcess();
};

extern HANDLE DeviceHandle;

#endif //GLOBLEHOTKEY4J_MESSAGEPROCESS_H
