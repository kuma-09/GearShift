//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/Resources.h"
#include "Framework/Audio.h"
#include "Game/PlayScene.h"
#include "Game/TitleScene.h"
#include "Game/DeferredRendering.h"
#include "Game/Manager/RenderManager.h"
#include "Game/Shader/PostProcess/Noise.h"
#include "Game/Shader/ShadowMap.h"
#include "Game/Shader/PostProcess/Bloom.h"
#include "Game/Manager/CollisionManager.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();
    m_resources = Resources::GetInstance();
    m_audio = Audio::GetInstance();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
    
}


void Game::Exit()
{
    ExitGame();
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
    m_audio->LoadSoundFile();
    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);

    m_scene = nullptr;
    m_titleScene = nullptr;
    m_playScene = nullptr;
    m_resultScene = nullptr;
    m_gameOverScene = nullptr;
    m_testScene = nullptr;
 
    m_sceneMask = std::make_unique<SceneMask>();

    m_debugString = std::make_unique<DebugString>(m_graphics->GetDeviceResources()->GetD3DDevice(),
        m_graphics->GetDeviceResources()->GetD3DDeviceContext(),
        L"Resources/Fonts/SegoeUI_18.spritefont");
    m_debugString->SetColor(DirectX::Colors::White);

    m_spriteBatch = std::make_unique<SpriteBatch>(m_deviceResources->GetD3DDeviceContext());

    DeferredRendering::Initialize();
    ShadowMap::Initialize();
    Noise::Initialize();
    Bloom::Initialize();
    GetTitleScene();
    GetPlayScene();
    GetResultScene();
    GetGameOverScene();


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

    m_inputManager->Update();
    m_audio->Update();
    Noise::Update(elapsedTime);
    //if (m_inputManager->GetKeyboardTracker()->pressed.Escape)
    //{
    //    ExitGame();
    //}

    // TODO: Add your game logic here.
    m_debugString->AddString(std::to_string(m_timer.GetFramesPerSecond()).c_str());
    m_scene->Update(elapsedTime);

    if (m_sceneMask->IsClose() && m_sceneMask->IsOpen()) return;
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
        else if (dynamic_cast<GameOverScene*>(m_scene))
        {
            m_gameOverScene.reset();
        }
        m_scene = nullptr;
        m_scene = m_tmpScene;
        m_scene->Initialize(this);
    }

    elapsedTime;

}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    using namespace DirectX::SimpleMath;

    auto state = m_graphics->GetCommonStates();
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();
    m_deviceResources->PIXBeginEvent(L"Render");

    // TODO: Add your rendering code here.

    // 現在のシーンを描画
    SceneRender();

    // シーン切り替え時のマスク
    if (m_sceneMask->IsClose() || m_sceneMask->IsOpen()) m_sceneMask->Render();

    #ifdef _DEBUG
    // デバッグ用
    DeferredRendering::GBufferShow();
    ShadowMap::ShadowMapShow();
    Bloom::BloomTextureShow();
    m_debugString->Render(state);
    #endif // DEBUG

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

void Game::SceneRender()
{
    // シャドウマップを作成
    CreateShadow();
    // ディファードレンダリング
    DeferredRendering();
    // 半透明オブジェクトの描画
    TranslucentRender();
    // ブルームを適用
    ApplyBloom();
    // ノイズを適用
    ApplyNoise();
    // UIの描画
    RenderUI();
}

void Game::CreateShadow()
{
    m_deviceResources->PIXBeginEvent(L"CreateShadow");
    RenderManager::CreateShadowMap();
    m_deviceResources->PIXEndEvent();
}

void Game::DeferredRendering()
{
    m_deviceResources->PIXBeginEvent(L"DeferredRendering");
    // DeferredRendering
    DeferredRendering::BeginGBuffer();
    //m_scene->Render();
    m_scene->Render();
    // GBufferを元に計算して描画
    m_deviceResources->PIXEndEvent();
    m_deviceResources->PIXBeginEvent(L"DeferredLighting");
    DeferredRendering::DeferredLighting();
    m_deviceResources->PIXEndEvent();
}

void Game::TranslucentRender()
{
    m_deviceResources->PIXBeginEvent(L"TranslucentObject");
    // 半透明オブジェクトを描画
    m_scene->TranslucentRender();
    m_deviceResources->PIXEndEvent();
}

void Game::RenderUI()
{
    m_deviceResources->PIXBeginEvent(L"UIRender");
    // ForwardRenderingでUIを表示
    m_scene->RenderUI();
    m_deviceResources->PIXEndEvent();
}

void Game::ApplyBloom()
{
    m_deviceResources->PIXBeginEvent(L"Bloom");
    // Bloomで光らせるオブジェクトを描画
    Bloom::BeginBloom();
    m_scene->TranslucentRender();
    m_deviceResources->PIXEndEvent();
    m_deviceResources->PIXBeginEvent(L"ApplyBloom");
    // Bloomを適用
    Bloom::EndBloom(DeferredRendering::GetFinalRenderTexture()->GetShaderResourceView());
    m_deviceResources->PIXEndEvent();
}

void Game::ApplyNoise()
{
    m_deviceResources->PIXBeginEvent(L"ApplyNoise");
    // ノイズを画面に適用
    Noise::ApplyNoise(Bloom::GetFinalRenderTexture()->GetShaderResourceView());
    m_deviceResources->PIXEndEvent();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // SceneRenderの方でClearしているのでコメントアウト--
    //// Clear the views.
    //auto context = m_deviceResources->GetD3DDeviceContext();
    //auto renderTarget = m_deviceResources->GetRenderTargetView();
    //auto depthStencil = m_deviceResources->GetDepthStencilView();

    //context->ClearRenderTargetView(renderTarget, Colors::Black);
    //context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    //context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    //// Set the viewport.
    //auto const viewport = m_deviceResources->GetScreenViewport();
    //context->RSSetViewports(1, &viewport);

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
    width = 1280;
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
        500.0f
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

void Game::ChangeScene(Scene* scene)
{
    if (!m_sceneMask->IsClose() && !m_sceneMask->IsOpen())
    {
        m_sceneMask->Close();
        m_tmpScene = scene;
    }
}

#pragma endregion
