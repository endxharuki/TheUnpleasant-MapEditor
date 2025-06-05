#pragma once
// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>


class ImguiManager
{
private:

    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    
    ID3D11RenderTargetView* m_RenderTargetView;
    IDXGISwapChain* m_pSwapChain = nullptr;
    bool   m_SwapChainOccluded = false;

    // Our state

    // Main loop
    bool done = false;


public:
    
    float FrameRate = 0.0f;

    ImguiManager() = default;
    ~ImguiManager() = default;
	void Init();
	void Uninit();

    void ImGuiRendererInit();
    void ImGuiRenderer();
	void DrawImGui();
};

