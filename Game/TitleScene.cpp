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
    m_titleLogo->Create(m_deviceResources, L"Resources/Textures/coolTitle.png", Vector2(x / 2, y / 2), Vector2::One, tito::MIDDLE_CENTER);
    m_titleLogo->SetWindowSize(x, y);
    
}


void TitleScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    UNREFERENCED_PARAMETER(elapsedTime);
    
    const auto& gp = m_inputManager->GetGamePadTracker();
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

    m_titleLogo->Render();

}

void TitleScene::Finalize()
{

}
