#include "pch.h"
#include "ResultScene.h"
#include "Game.h"


ResultScene::ResultScene()
{
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();


    m_result = std::make_unique<UI>(L"Resources/Textures/StageClear.png");
    m_backGround = std::make_unique<UI>(L"Resources/Textures/haguruma.png");
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

    int x, y;
    Graphics::GetInstance()->GetScreenSize(x,y);

    m_backGround->Render(Vector2::Zero);
    m_result->Render(Vector2(x / 3.5f, y / 2.5f));

}

void ResultScene::Finalize()
{

}
