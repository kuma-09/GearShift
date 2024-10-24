#include "pch.h"
#include "ResultScene.h"
#include "Game.h"


ResultScene::ResultScene()
{
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    int x, y;
    m_graphics->GetScreenSize(x, y);

    m_result = std::make_unique<UI>(L"Resources/Textures/StageClear.png", Vector2(x / 3.f, y / 3.f));
    m_backGround = std::make_unique<UI>(L"Resources/Textures/haguruma.png", Vector2::Zero);
}

ResultScene::~ResultScene()
{
    
}

void ResultScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    SetGame(game);
    
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
