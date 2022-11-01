#include <iostream>
#include <windows.h>

#include "../include/device.h"
#include "../include/MessageProcess.h"
#include "../include/Stc.h"
#include "../include/tWind.h"
#include "kdmapperlib.h"

bool isFileExists(const std::wstring& name) {
    if (FILE *file = _wfopen(name.c_str(), L"r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

bool kdMapperLoad()
{
    DeviceHandle = CreateFile(LINKNAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DeviceHandle != INVALID_HANDLE_VALUE)
    {
        std::cout << "Drive has been mapped" <<std::endl;
        return true;
    }
    WCHAR exeFullPath[MAX_PATH];
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
    std::wstring exepath = exeFullPath;
    exepath = exepath.substr(0,exepath.find_last_of(L"\\",exepath.length()));
    std::wstring filepath = exepath + L"\\libs\\";
    filepath +=MAPSYSFILENAME;
    if (!isFileExists(filepath))
    {
        MessageBox(NULL,L"map sys文件不存在",L"0x11",MB_OK);
        return false;
    }
    if (load(filepath.c_str()))
    {
        DeviceHandle = CreateFile(LINKNAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (DeviceHandle == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        atexit([](){
            CloseHandle(DeviceHandle);
        });
        return true;
    }
    return false;
}

void normalLoad()
{
    WCHAR exeFullPath[MAX_PATH];
    GetModuleFileName(NULL,exeFullPath,MAX_PATH);
    std::wstring exepath = exeFullPath;
    exepath = exepath.substr(0,exepath.find_last_of(L"\\",exepath.length()));

    std::wstring filepath = exepath + L"\\libs\\";
    filepath +=SYSFILENAME;
    if (!isFileExists(filepath))
    {
        MessageBox(NULL,L"sys文件不存在",L"0x10",MB_OK);
        exit(0);
    }

    TCHAR  buffer[1024]=TEXT("");
    TCHAR* lppPart={NULL};
    auto b0 = GetFullPathName(filepath.c_str(),
                              1024,
                              buffer,
                              &lppPart);
    if (b0 == 0)
    {
        std::cout << "error code " << GetLastError() << std::endl;
        MessageBox(NULL,L"sys文件不存在",L"0x10",MB_OK);
        exit(-1);
    }
    std::wstring fullpath = buffer;
    fullpath = L"\\??\\"+fullpath;
    std::wcout << L"sys path is [" << fullpath << L"]" << std::endl;
    unloadDevice(R3NAME);
    auto b1 = installDevice(fullpath.c_str(),R3NAME);
    if(b1==FALSE)
    {
        std::cout << "加载失败 0x0 code:" << GetLastError() << std::endl;
        MessageBox(NULL,L"加载失败",L"0x0",MB_OK);
        exit(-1);
    }

    auto b2 = startDvr(R3NAME);
    if(b2==FALSE)
    {
        std::cout << "加载失败 0x1 code:" << GetLastError() << std::endl;
        MessageBox(NULL,L"加载失败",L"0x1",MB_OK);
        exit(-1);
    }
    std::cout << "device load success" <<   std::endl;
    DeviceHandle = CreateFile(LINKNAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DeviceHandle == INVALID_HANDLE_VALUE)
    {
        std::cout << "加载失败 0x2 code:" << GetLastError() << std::endl;
        MessageBox(NULL,L"加载失败",L"0x2",MB_OK);
        exit(-1);
    }

    atexit([](){
        CloseHandle(DeviceHandle);
        stopDvr(R3NAME);
        unloadDevice(R3NAME);
        std::cout << "device unload success" <<   std::endl;
    });
}

void checkVersionAndSetVulnerableDriverBlocklistEnable()
{
    bool restart = false;
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\CI\Config
    DWORD d = 999;
    DWORD size = sizeof(DWORD);
    int i = RegGetValueW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\CI\\Config",
                         L"VulnerableDriverBlocklistEnable", RRF_RT_REG_DWORD, 0, &d, &size);
    if (i!=0)
    {
        return;
    }

    if (d != 0 && size == 4 && d!=999)
    {
        d = 0;
        size = sizeof(DWORD);
        i = RegSetKeyValueW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\CI\\Config",
                        L"VulnerableDriverBlocklistEnable",REG_DWORD,&d,size);
        if  (i != 0)
        {
        } else
        {
            restart = true;
        }
    }

    if (restart == true)
    {
        if (MessageBoxW(NULL,L"请重启系统后使用,是否立即重启",L"提示",MB_ICONEXCLAMATION | MB_OKCANCEL)==IDOK)
        {
            system("shutdown -s -t 10");
        }
        exit(0);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            HANDLE hMutex = NULL;
            hMutex = CreateMutex(NULL, FALSE, TEXT("GenshinImpactPianoOnlyOne"));
            if (hMutex == NULL || ERROR_ALREADY_EXISTS == GetLastError())
            {
                MessageBox(NULL, TEXT("程序只能运行一个"), TEXT("Error"), MB_OK);
                exit(0);
            }
            if (!IsUserAnAdmin())
            {
                MessageBox(NULL,TEXT("请以管理员运行"),TEXT(""),MB_OK);
                exit(0);
            }

            checkVersionAndSetVulnerableDriverBlocklistEnable();
            if (!kdMapperLoad())
            {
                MessageBoxW(NULL,L"map驱动失败,使用正常方式加载,请关闭系统签名验证",L"提示",MB_OK);
                normalLoad();
            }

            if (DeviceHandle == NULL || DeviceHandle == INVALID_HANDLE_VALUE)
            {
                MessageBoxW(NULL,L"加载失败请重启后再试",L"提示",MB_OK);
                exit(0);
            }

            tWind::getInstance()->startSync();
            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

int main(int, char**) {
    //DllMain(0,DLL_PROCESS_ATTACH,0);
}

