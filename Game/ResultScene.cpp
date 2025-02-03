#include "pch.h"
#include "ResultScene.h"
#include "Game.h"
#include "Manager/StageDataManager.h"

ResultScene::ResultScene()
{
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();


    m_result = std::make_unique<UI>(L"Resources/Textures/StageClear.png");
    m_backGround = std::make_unique<UI>(L"Resources/Textures/haguruma.png");

    m_clearTime = std::make_unique<UI>(L"Resources/Textures/ClearTime.png");

    m_time = std::make_unique<Number>();
    m_time->Initialize({ 640,200 });
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    SetGame(game);
    int clearTime = StageDataManager::GetClearTime();
    m_time->SetNum(clearTime);
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
}

void ResultScene::RenderUI()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    int x, y;
    Graphics::GetInstance()->GetScreenSize(x, y);

    m_backGround->Render(Vector2::Zero);
    m_result->Render(Vector2(100,340));
    m_clearTime->Render(Vector2(100, 150),DirectX::Colors::White,
    Vector2::Zero,Vector2{0.5f,0.5f});
    m_time->RenderTime();
}

void ResultScene::Finalize()
{

}
