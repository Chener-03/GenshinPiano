//
// Created by chen on 2022/10/11.
//

#include "../include/MessageProcess.h"
#include <iostream>


MessageProcess *MessageProcess::instance= nullptr;

HANDLE DeviceHandle = NULL;

LRESULT ProxyDispatchMessageW(const MSG *lpMsg)
{
    LRESULT res = 0;
    auto ins = MessageProcess::getInstance();
    if (ins->realPtr == nullptr)
        return res;

    for (const auto &item: ins->ef)
    {
        if(item.event == lpMsg->wParam && item.winHwnd == lpMsg->hwnd)
        {
            if (item.function(item.e,item.method,item.obj))
            {
                return res;
            }
        }
    }
    
    res = ((RealDispatchMessageFun)ins->realPtr)(lpMsg);
    return res;
}



bool MessageProcess::addEvent(int event, int hwnd ,FUNCTION callback,JNIEnv * e,jmethodID i,jobject obj)
{
    EventFunctionMapper eventFunctionMapper{};
    eventFunctionMapper.event = event;
    eventFunctionMapper.function = callback;
    eventFunctionMapper.winHwnd = (HWND)hwnd;
    eventFunctionMapper.e = e;
    eventFunctionMapper.method = i;
    eventFunctionMapper.obj = obj;

    ef.push_back(eventFunctionMapper);
    return true;
}


bool MessageProcess::startHook() {
    auto ins = MessageProcess::getInstance();
    auto status = MH_CreateHook(DispatchMessageW, ProxyDispatchMessageW,&ins->realPtr);
    if (status==MH_STATUS::MH_OK)
    {
        status = MH_EnableHook(MH_ALL_HOOKS);
        std::cout << "start hook" <<  std::endl;
    }
    return status == MH_STATUS::MH_OK;
}

MessageProcess::MessageProcess() {
    MH_Initialize();
}

MessageProcess::~MessageProcess() {
    MH_Uninitialize();
}

bool MessageProcess::stopHook() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(DispatchMessageW);
    std::cout << "stop hook" <<  std::endl;
    return true;
}

int MessageProcess::addHotKey(char *hotKeyName, int hwnd, int modifiers, int key)
{
    std::string cstr = hotKeyName;
    std::wstring wstr(cstr.begin(),cstr.end());
    auto hkid = GlobalAddAtom(wstr.c_str());
    if (RegisterHotKey((HWND) hwnd, hkid, modifiers, key))
        return hkid;
    else return -1;
}

void MessageProcess::deleteHotKey(int hwnd,int id)
{
    UnregisterHotKey((HWND) hwnd,id);
}

bool MessageProcess::deleteEvent(int event) {
    bool res = false;
    for (auto it = ef.begin(); it!=ef.end() ; ++it) {
        auto item = (*it);
        if (item.event == event)
        {
            res = true;
            item.e->DeleteGlobalRef(item.obj);
            ef.erase(it);
            break;
        }
    }
    return res;
}
