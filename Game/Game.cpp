//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/Resources.h"
#include "Game/PlayScene.h"
#include "Game/TitleScene.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();
    m_resources = Resources::GetInstance();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{

}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    using namespace DirectX::SimpleMath;

    m_hWnd = window;
    m_graphics->SetScreenSize(width, height);
    m_deviceResources->SetWindow(m_hWnd, width, height);


    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_inputManager->Initialize(window);
    m_resources->LoadResource(m_graphics);
    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);

    m_scene = nullptr;
    m_titleScene = nullptr;
    m_playScene = nullptr;
    m_resultScene = nullptr;
    m_testScene = nullptr;
 
    m_sceneMask = std::make_unique<SceneMask>();

    //ChangeScene(GetTitleScene());
    m_scene = GetTitleScene();
    m_scene->Initialize(this);

}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    using namespace DirectX::SimpleMath;
    float elapsedTime = float(timer.GetElapsedSeconds());

    // FPSデバッグ表示
    OutputDebugString(std::to_wstring(timer.GetFramesPerSecond()).c_str());
    OutputDebugString(L"\n");

    m_inputManager->Update();

    if (m_sceneMask->IsClose() || m_sceneMask->IsOpen())
    {
        if (m_sceneMask->Update(elapsedTime))
        {
            m_scene->Finalize();
            if (dynamic_cast<PlayScene*>(m_scene))
            {
                m_playScene.reset();
            }
            else if (dynamic_cast<TitleScene*>(m_scene))
            {
                m_titleScene.reset();
            }
            else if (dynamic_cast<ResultScene*>(m_scene))
            {
                m_resultScene.reset();
            }
            m_scene = nullptr;

            m_scene = m_tmpScene;
            m_scene->Initialize(this);
        }
    }

    // TODO: Add your game logic here.

    m_scene->Update(elapsedTime);

    elapsedTime;

}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    using namespace DirectX::SimpleMath;

    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");

    // TODO: Add your rendering code here.
    m_scene->Render();

    if (m_sceneMask->IsClose() || m_sceneMask->IsOpen())
    {
        m_sceneMask->Render();
    }


    m_deviceResources->PIXEndEvent();


    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1200;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();


    // TODO: Initialize device dependent objects here (independent of window size).
    device;

    // デバイスに依存したオブジェクトを初期化する
    int width, height;
    // ウィンドウの既定サイズを取得する
    GetDefaultSize(width, height);
    // 「ウィンドウハンドル」「幅」「高さ」を設定する
    m_graphics->GetDeviceResources()->SetWindow(m_hWnd, width, height);
    // グラフィックスを初期化する
    m_graphics->Initialize();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

    // 出力サイズを取得する
    auto m_size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(m_size.right) / float(m_size.bottom);
    // 視野角を設定する
    float fovAngleY = 45.0f * DirectX::XM_PI / 180.0f;

    DirectX::SimpleMath::Matrix projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        fovAngleY,
        aspectRatio,
        0.1f,
        1000.0f
    );
    // 射影行列を設定する
    m_graphics->SetProjectionMatrix(projection);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}

void Game::ChangeScene(IScene* scene)
{

    m_sceneMask->Close();
    m_tmpScene = scene;

}

#pragma endregion
