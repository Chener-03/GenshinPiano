#include <iostream>
#include <windows.h>

#include "../include/device.h"
#include "../include/MessageProcess.h"
#include "../include/Stc.h"
#include "../include/tWind.h"

bool isFileExists(const std::wstring& name) {
    if (FILE *file = _wfopen(name.c_str(), L"r")) {
        fclose(file);
        return true;
    } else {
        return false;
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

}

