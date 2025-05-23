#include "pch.h"
#include "StageSelectScene.h"
#include "Game.h"
#include "Framework/Resources.h"
#include "Framework/BinaryFile.h"
#include "Framework/Easing.h"

void StageSelectScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    SetGame(game);

    m_skydome = std::make_unique<SkyDome>();
    m_skydome->Initialize();
    m_skydome->SetWorld(Matrix::CreateTranslation(Vector3(0, 0, 0)));

    m_player = std::make_unique<TitlePlayer>(this);
    m_player->SetPosition(Vector3{ 0,2.4,0 });
    m_player->Initialize();


    m_camera = std::make_unique<TitleCamera>();
    m_camera->Initialize(m_player.get());
    m_camera->SetPosition(Vector3{ 0,5,-5 });

    m_menu = std::make_unique<Menu>();
    m_menu->AddUI(L"Resources/Textures/Stage1.png", { 0,150 });
    m_menu->AddUI(L"Resources/Textures/Stage2.png", { 0,300 });
    m_menu->AddUI(L"Resources/Textures/Stage3.png", { 0,450 });
    m_menu->Initialize();
    m_menuBack = std::make_unique<UI>(L"Resources/Textures/SceneChangeBlack.png");
    m_menuBack->Initialize();
    m_nowTime = 0;
    m_isStageSelect = false;
}


void StageSelectScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    m_skydome->Update(elapsedTime);
    m_camera->Update(elapsedTime);
    m_player->Update(elapsedTime);

    if (m_isStageSelect)
    {
        m_nowTime += elapsedTime;
        m_menu->Update();
        m_menuPosition = Easing::InOutQuart(m_nowTime, 1.0f);
        if (kb->pressed.Space || gp->a)
        {
            GetGame()->ChangeScene(GetGame()->GetPlayScene());
        }
    }

    if (kb->pressed.Space || gp->a)
    {
        m_isStageSelect = true;
    }
}

void StageSelectScene::Render()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto proj = m_graphics->GetProjectionMatrix();


    if (m_isStageSelect)
    {
        m_menuBack->Render({ m_menuPosition * 1280 - 640,720 / 2 }, { 1,1,1,0.5f }, Vector2{ 1280,720 } / 2, { 0.95f,0.95f });
    }
    m_menu->Render(Vector2{ m_menuPosition * 1280 - 640,0 });

}

void StageSelectScene::Finalize()
{

}
