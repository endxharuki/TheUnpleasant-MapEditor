// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include "main.h"
#include "renderer.h"
#include "imguiManager.h"

void ImguiManager::Init()
{
    m_Device = Renderer::GetDevice();
    m_DeviceContext = Renderer::GetDeviceContext();
    m_RenderTargetView = Renderer::GetRenderTagetView();
    m_pSwapChain = Renderer::GetSwapChain();

    //‚±‚±‚©‚ç‰º
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    // Setup Dear ImGui style
    
    
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(GetWindow());
    ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
    ImGui::StyleColorsDark();

    

}

void ImguiManager::Uninit()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

}

void ImguiManager::ImGuiRendererInit()
{
   /* MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            OnDone();
    }*/

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    FrameRate = ImGui::GetIO().Framerate;
}

void ImguiManager::ImGuiRenderer()
{

    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void ImguiManager::DrawImGui()
{
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            /*if (msg.message == WM_QUIT)
                done = true;*/
        }
        // Start the Dear ImGui frame
        
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
       /* if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);*/

        float i = 0.0f;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        
            //‚±‚±‚ç•ÓŒ©‚Æ‚¯
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("f = %f", f);

        ImGui::End();
        

        // Rendering
        ImGui::Render();
        /*const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
        m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clear_color_with_alpha);*/
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        //HRESULT hr = m_pSwapChain->Present(0, NULL);   // Present with vsync
        ////HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        //m_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

}
