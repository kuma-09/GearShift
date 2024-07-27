#include "pch.h"
#include "ResultScene.h"
#include "Game.h"


void ResultScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    SetGame(game);

    int x, y;
    m_graphics->GetScreenSize(x, y);

    m_result = std::make_unique<tito::UserInterface>();
    m_result->Create(m_deviceResources, L"Resources/Textures/StageClear.png", Vector2(x / 2.0f, y / 2.0f), Vector2::One * 2, tito::MIDDLE_CENTER);
    m_result->SetWindowSize(x, y);

    m_backGround = std::make_unique<tito::UserInterface>();
    m_backGround->Create(m_deviceResources, L"Resources/Textures/haguruma.png", Vector2(x / 2.0f, y / 2.0f), Vector2::One, tito::MIDDLE_CENTER);
    m_backGround->SetWindowSize(x, y);
    
}


void ResultScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    UNREFERENCED_PARAMETER(elapsedTime);
    
    //const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    if (kb->pressed.Space)
    {
        GetGame()->ChangeScene(GetGame()->GetTitleScene());
    }


}

void ResultScene::Render()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_backGround->Render();
    m_result->Render();

}

void ResultScene::Finalize()
{

}
