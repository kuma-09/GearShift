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

class Graphics;
class InputManager;
class Resources;


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

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
            return m_titleScene;
        }
        return m_titleScene = new TitleScene();
    }

    PlayScene* GetPlayScene() 
    {
        if (m_playScene)
        {
            return m_playScene;
        }
        return m_playScene = new PlayScene();
    }

    ResultScene* GetResultScene() 
    {
        if (m_resultScene)
        {
            return m_resultScene;
        }
        return m_resultScene = new ResultScene();
    }

private:

    void Update(DX::StepTimer const& timer);
    void Render();

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


    IScene* m_scene;
    PlayScene* m_playScene;
    TitleScene* m_titleScene;
    ResultScene* m_resultScene;

};
