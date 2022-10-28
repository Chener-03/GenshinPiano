//
// Created by chen on 2022/10/19.
//

#ifndef GLOBLEHOTKEY4J_TWIND_H
#define GLOBLEHOTKEY4J_TWIND_H

#include <iostream>
#include <mutex>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <dwmapi.h>
#include <d2d1.h>
#include <dwrite.h>
#include <list>
#include "../include/GameVer.h"
#include "jni.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct JavaFunctionPointer{
    JavaVM* jvm = NULL;
    jmethodID method = NULL;
    jobject obj = NULL;
};

static std::mutex mutex_tWind;
class tWind {
public:
    static tWind* getInstance()
    {
        if (ptWind == nullptr)
        {
            std::lock_guard<std::mutex> lock(mutex_tWind);
            if (ptWind == nullptr)
            {
                ptWind = new tWind();
            }
        }
        return ptWind;
    }


public:
    ID3D11Device*               g_pd3dDevice = NULL;
    ID3D11DeviceContext*        g_pd3dDeviceContext = NULL;
    IDXGISwapChain*             g_pSwapChain = NULL;
    ID3D11RenderTargetView*     g_mainRenderTargetView = NULL;

    ID2D1Factory*               pd2dfactory = NULL;
    ID2D1RenderTarget*          pd2dRenderTarget = NULL;
    IDWriteFactory*             pd2dWriteFactory = NULL;
    IDWriteFont*                pd2dFont = NULL;
    IDWriteTextFormat*          pd2dTextFormat = NULL;

    HWND                     hwnd;
    bool running = true;
    WNDCLASSEXW wc = { 0 };


    static tWind* ptWind;
    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 0.00f);
    COLORREF ctColor = RGB(0, 0, 0);
    HANDLE thread = 0;
    HWND genshinHwnd = NULL;
    bool windowIsShow = true;
    std::list<JavaFunctionPointer> drawCallbackFunctions;
    std::mutex drawFunctionsMutex;

    tWind();
    ~tWind();
    VOID startSync();
    VOID startInConcurrentThread();
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    bool moveToGenshin();
    void CreateD2DRenderTarget();
    inline void addDrawCallbackFunction(JavaFunctionPointer ptr)
    {
        drawCallbackFunctions.push_back(ptr);
    }
};



#endif //GLOBLEHOTKEY4J_TWIND_H
