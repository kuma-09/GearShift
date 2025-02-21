//
// Game.h
//

#pragma once
#include "Framework/DeviceResources.h"
#include "Framework/StepTimer.h"
#include "Game/IScene.h"
#include "Game/PlayScene.h"
#include "Game/TitleScene.h"
#include "Game/ResultScene.h"
#include "Game/GameOverScene.h"
#include "Game/TestScene.h"
#include "SceneMask.h"
#include "Shader/OutLine.h"
#include "Framework/DebugString.h"

class Graphics;
class InputManager;
class Resources;
class Audio;


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    void Exit();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

    void ChangeScene(IScene* scene);

    TitleScene* GetTitleScene()
    {
        if (m_titleScene)
        {
            return m_titleScene.get();
        }
        m_titleScene = std::make_unique<TitleScene>();
        return m_titleScene.get();
    }

    PlayScene* GetPlayScene() 
    {
        if (m_playScene)
        {
            return m_playScene.get();
        }
        m_playScene = std::make_unique<PlayScene>();
        return m_playScene.get();
    }

    ResultScene* GetResultScene() 
    {
        if (m_resultScene)
        {
            return m_resultScene.get();
        }
        m_resultScene = std::make_unique<ResultScene>();
        return m_resultScene.get();
    }

    GameOverScene* GetGameOverScene()
    {
        if (m_gameOverScene)
        {
            return m_gameOverScene.get();
        }
        m_gameOverScene = std::make_unique<GameOverScene>();
        return m_gameOverScene.get();
    }

    TestScene* GetTestScene()
    {
        if (m_resultScene)
        {
            return m_testScene.get();
        }
        m_testScene = std::make_unique<TestScene>();
        return m_testScene.get();
    }

    // フルスクリーン関連
private:
    // ★追記
    BOOL m_fullScreen;

public:
    // 追記
    // 画面モードを変更する関数（TRUE:フルスクリーン）
    void SetFullscreenState(BOOL value)
    {
        m_fullScreen = value;
        m_deviceResources->GetSwapChain()->SetFullscreenState(value, nullptr);
        if (value) m_deviceResources->CreateWindowSizeDependentResources();
    }

private:

    void Update(DX::StepTimer const& timer);
    void Render();
    void SceneRender();

    void CreateShadow();
    void DeferredRendering();
    void TranslucentRender();
    void RenderUI();
    void ApplyBloom();
    void ApplyNoise();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    
    HWND m_hWnd;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

    Graphics* m_graphics;
    DX::DeviceResources* m_deviceResources;
    InputManager* m_inputManager;
    Resources* m_resources;
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    Audio* m_audio;


    IScene* m_scene;
    IScene* m_tmpScene;
    std::unique_ptr<PlayScene> m_playScene;
    std::unique_ptr<TitleScene> m_titleScene;
    std::unique_ptr<ResultScene> m_resultScene;
    std::unique_ptr<GameOverScene> m_gameOverScene;
    std::unique_ptr<TestScene> m_testScene;
    std::unique_ptr<DebugString> m_debugString;

    std::unique_ptr<SceneMask> m_sceneMask;
    std::unique_ptr<PostOutLine> m_outLine;

};
