#include "pch.h"
#include "TitleScene.h"
#include "Game.h"


void TitleScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    SetGame(game);

    int x, y;
    m_graphics->GetScreenSize(x, y);

    m_titleLogo = std::make_unique<tito::UserInterface>();
    m_titleLogo->Create(m_deviceResources, L"Resources/Textures/GearShiftLogo.png", Vector2(x / 2.0f, y / 2.0f), Vector2::One * 2, tito::MIDDLE_CENTER);
    m_titleLogo->SetWindowSize(x, y);

    m_backGround = std::make_unique<tito::UserInterface>();
    m_backGround->Create(m_deviceResources, L"Resources/Textures/haguruma.png", Vector2(x / 2.0f, y / 2.0f), Vector2::One, tito::MIDDLE_CENTER);
    m_backGround->SetWindowSize(x, y);
    
}


void TitleScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    UNREFERENCED_PARAMETER(elapsedTime);
    
    //const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    if (kb->pressed.Space)
    {
        GetGame()->ChangeScene(GetGame()->GetPlayScene());
    }


}

void TitleScene::Render()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_backGround->Render();
    m_titleLogo->Render();

}

void TitleScene::Finalize()
{

}
