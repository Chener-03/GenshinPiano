//
// Created by chen on 2022/10/19.
//

#include "../include/tWind.h"

tWind *tWind::ptWind= nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return 1;
    auto ins = tWind::getInstance();
    switch (msg)
    {
        case WM_CREATE:
        {
            MARGINS margins = {-1,-1,-1,-1};
            DwmExtendFrameIntoClientArea(ins->hwnd,&margins);
            DWM_BLURBEHIND bb = {0} ;
            DwmExtendFrameIntoClientArea(ins->hwnd, &margins);
            bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
            bb.fEnable = true;
            bb.hRgnBlur = NULL;
            DwmEnableBlurBehindWindow(ins->hwnd, &bb);
            return 0;
        }

        case WM_SIZE:
            if (ins->g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
            {
                ins->CleanupRenderTarget();
                ins->g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                ins->CreateRenderTarget();
                ins->CreateD2DRenderTarget();
            }
            return 0;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
    }
    return (int)(::DefWindowProc(hWnd, msg, wParam, lParam));
}

VOID draw()
{
    while (tWind::getInstance()->running)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                tWind::getInstance()->running = false;
        }
        if (!tWind::getInstance()->running)
            break;


        if (!tWind::getInstance()->moveToGenshin())
        {
            Sleep(200);
        }

        {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            if(IsWindowVisible(tWind::getInstance()->hwnd))
            {
                ImGui::Begin("Genshin");
                ImGui::Text("App (%.1f FPS)",  ImGui::GetIO().Framerate);

                ImGui::ColorEdit3("button color", (float*)&(GVer::concurrentButtonClolr));

                ImGui::SliderFloat("button size",(float*)&(GVer::buttonSize),10,60);

                const char* items[] = { "1440X900" };
                ImGui::Combo("resolving power",&(GVer::fbl),items, IM_ARRAYSIZE(items));

                if (ImGui::Button(GVer::screenshotHidden?"close screenshot hidden":"open screenshot hidden"))
                {
                    if (GVer::screenshotHidden)
                    {
                        SetWindowDisplayAffinity(tWind::getInstance()->hwnd,WDA_NONE);
                        GVer::screenshotHidden = false;
                    } else
                    {
                        SetWindowDisplayAffinity(tWind::getInstance()->hwnd,WDA_EXCLUDEFROMCAPTURE);
                        GVer::screenshotHidden = true;
                    }

                }

                ImGui::Checkbox("show key mapper",&(GVer::showKeyMap));
                ImGui::ColorEdit3("key color",(float*)&(GVer::showKeyMapClolr));

                ImGui::End();
            }
            ImGui::Render();
            const float clear_color_with_alpha[4] = { tWind::getInstance()->clear_color.x * tWind::getInstance()->clear_color.w, tWind::getInstance()->clear_color.y * tWind::getInstance()->clear_color.w, tWind::getInstance()->clear_color.z * tWind::getInstance()->clear_color.w, tWind::getInstance()->clear_color.w };
            tWind::getInstance()->g_pd3dDeviceContext->OMSetRenderTargets(1, &(tWind::getInstance()->g_mainRenderTargetView), NULL);
            tWind::getInstance()->g_pd3dDeviceContext->ClearRenderTargetView(tWind::getInstance()->g_mainRenderTargetView, clear_color_with_alpha);

            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            if (tWind::getInstance()->pd2dRenderTarget != nullptr)
            {
                tWind::getInstance()->pd2dRenderTarget->BeginDraw();
                std::lock_guard<std::mutex> guard(tWind::getInstance()->drawFunctionsMutex);
                for (auto f = tWind::getInstance()->drawCallbackFunctions.begin();f != tWind::getInstance()->drawCallbackFunctions.end();f++)
                {
                    if((*f).jvm!=NULL && (*f).obj!= NULL && (*f).method!= nullptr)
                    {
                        JNIEnv * en;
                        int status = (*f).jvm->GetEnv((void**) &en, JNI_VERSION_1_4);
                        if (status == JNI_EDETACHED || en == NULL) {
                            status = (*f).jvm->AttachCurrentThread((void**)&en, NULL);
                            if (status < 0) {
                                break;
                            }
                        }
                        if(en != NULL) en->CallVoidMethod((*f).obj,(*f).method);
                    }
                }
                tWind::getInstance()->pd2dRenderTarget->EndDraw() ;
            }


            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }

            tWind::getInstance()->g_pSwapChain->Present(1, 0);
        }
    }

}

tWind::tWind() {

}

bool tWind::CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count =1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    UINT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE,
                                NULL, createDeviceFlags,
                                featureLevelArray, 2,
                                D3D11_SDK_VERSION,&g_pd3dDevice,
                                &featureLevel, &g_pd3dDeviceContext);
    if (hr != S_OK)
        return false;

    UINT m4x;
    g_pd3dDevice->CheckMultisampleQualityLevels(
            DXGI_FORMAT_B8G8R8A8_UNORM,8,&m4x);
    if(m4x > 0)
    {
        sd.SampleDesc.Count =4;
        sd.SampleDesc.Quality = m4x - 1;
    }


    IDXGIDevice* dxgiDevice = nullptr;
    IDXGIAdapter* dxgiAdapter = nullptr;
    IDXGIFactory* dxgiFactory = nullptr;
    g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
    hr = dxgiFactory->CreateSwapChain(g_pd3dDevice,&sd, &g_pSwapChain);
    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();
    if (hr != S_OK)
        return false;

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&pd2dfactory);
    if (hr != S_OK)
        return false;

    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                        reinterpret_cast<IUnknown**>(&pd2dWriteFactory));
    hr =  pd2dWriteFactory->CreateTextFormat(L"微软雅黑", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
                                      DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"zh-cn",
                                      &pd2dTextFormat);
    //pd2dWriteFactory->CreateFontFileReference()

    if (hr != S_OK)
        return false;

    CreateD2DRenderTarget();
    CreateRenderTarget();
    return true;
}

void tWind::CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void tWind::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void tWind::CleanupRenderTarget() {
    if (pd2dRenderTarget)
    {
        pd2dRenderTarget->Release();
        pd2dRenderTarget = NULL;
    }
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

tWind::~tWind() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

VOID tWind::startInConcurrentThread() {
    ImGui_ImplWin32_EnableDpiAwareness();

    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(NULL);

    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.lpszClassName = L"hidden0psk3298a2";
    wc.lpszMenuName = L" ";
    wc.style = CS_VREDRAW | CS_HREDRAW;

    ::RegisterClassExW(&wc);
    hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, L"hidden0psk3298a2", L" ", WS_POPUP, 1, 1, 1440, 900, 0, 0, wc.hInstance, 0);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        std::cout << "CreateDeviceD3D ERROR WITH CODE "<< GetLastError() << std::endl;
        exit(0);
    }
    SetLayeredWindowAttributes(hwnd, ctColor , 0 , LWA_COLORKEY);
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 0.8f;
    }

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    io.Fonts->AddFontFromFileTTF("C:\\Users\\chen\\Downloads\\imgui-master\\GenshinFont.ttf",16.0f);
    draw();
}

VOID tWind::startSync() {
    void* fun = [](){
        tWind::getInstance()->startInConcurrentThread();
    };
    thread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)fun,NULL,NULL,NULL);
}

bool tWind::moveToGenshin() {
    RECT r = {0};
    GetWindowRect(hwnd,&r);
    if (genshinHwnd!=NULL && IsWindow(genshinHwnd))
    {
        WCHAR wn[256] = {0};
        GetWindowTextW(genshinHwnd,wn,256);
        if (lstrcmp(wn,L"原神") != 0)
        {
            genshinHwnd = NULL;
            return false;
        }
    } else
    {
        genshinHwnd = FindWindow(NULL,TEXT("原神"));
        if (genshinHwnd == NULL)
        {
            ShowWindow(hwnd,SW_HIDE);
            windowIsShow = false;
            return false;
        }
    }


    if (!IsIconic(genshinHwnd))
    {
        RECT gsRect = {0};
        GetWindowRect(genshinHwnd,&gsRect);
        DWORD dwStyle = GetWindowLong(genshinHwnd, GWL_STYLE);
        if (dwStyle & WS_BORDER)
        {
            gsRect.top += 26;
            gsRect.left += 3;
        }

        MoveWindow(hwnd,gsRect.left,gsRect.top,gsRect.right-gsRect.left,gsRect.bottom-gsRect.top,false);
        if (!windowIsShow)
        {
            ShowWindow(hwnd,SW_NORMAL);
            windowIsShow = true;
        }
        return true;
    }
    ShowWindow(hwnd,SW_HIDE);
    windowIsShow = false;
    return false;
}

void tWind::CreateD2DRenderTarget() {
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_HARDWARE,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

    IDXGISurface* surface = NULL;
    g_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&surface));
    pd2dfactory->CreateDxgiSurfaceRenderTarget(surface,&props,&pd2dRenderTarget);
    pd2dRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    surface->Release();
}
