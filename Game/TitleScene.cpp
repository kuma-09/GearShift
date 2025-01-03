#include "pch.h"
#include "TitleScene.h"
#include "Game.h"
#include "Game/Components/Camera.h" 
#include "Framework/Resources.h"
#include "Framework/BinaryFile.h"
#include "Framework/Easing.h"

#include "Manager/RenderManager.h"
#include "Manager/StageDataManager.h"

void TitleScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    SetGame(game);
 


    m_titleLogo = std::make_unique<UI>(L"Resources/Textures/GearShiftLogo.png");
    m_titleLogo->Initialize();

    m_skydome = std::make_unique<SkyDome>();
    m_skydome->Initialize({ 0,-20,0 });
    
    m_player = std::make_unique<TitlePlayer>(this);
    m_player->SetPosition(Vector3{ 0,2.4,0 });
    m_player->Initialize();
    
    m_camera = std::make_unique<TitleCamera>();
    m_camera->Initialize(m_player.get());
    m_camera->SetPosition(Vector3{ 0,5,-5 });

    m_floor = std::make_unique<Floor>(this);


    m_menu = std::make_unique<Menu>();
    m_menu->AddUI(L"Resources/Textures/Start.png", {0,450}, {0.5f,0.5f});
    m_menu->AddUI(L"Resources/Textures/Option.png",{0,550}, {0.5f,0.5f});
    m_menu->AddUI(L"Resources/Textures/Exit.png",  {0,650}, {0.5f,0.5f});
    m_menu->Initialize();

    m_stageMenu = std::make_unique<Menu>();
    m_stageMenu->AddUI(L"Resources/Textures/Stage1.png", { 0,150 }, { 1.f,1.f });
    m_stageMenu->AddUI(L"Resources/Textures/Stage2.png", { 0,350 }, { 1.f,1.f });
    m_stageMenu->AddUI(L"Resources/Textures/Stage3.png", { 0,550 }, { 1.f,1.f });
    m_stageMenu->Initialize();

    m_menuBack = std::make_unique<UI>(L"Resources/Textures/SceneChangeBlack.png");
    m_menuBack->Initialize();
    m_nowTime = 0;
    m_stageNowTime = 0;
    m_isStageSelect = false;
}


void TitleScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;
    
    const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    m_skydome->Update(elapsedTime);
    m_camera->Update(elapsedTime);
    m_player->Update(elapsedTime);


    m_nowTime += elapsedTime;
    if (m_isStageSelect)
    {
        m_stageNowTime += elapsedTime;
        m_stageMenuPosition = Easing::InOutQuart(m_stageNowTime, 1.0f);
        m_stageMenu->Update();
    }
    else
    {
        m_menu->Update();
    }
    m_menuPosition = Easing::InOutQuart(m_nowTime, 1.0f);
    if (kb->pressed.Space || gp->a)
    {
        if (m_isStageSelect)
        {
            StageDataManager::SetStageNum(m_stageMenu->GetActiveUI() + 1);
            GetGame()->ChangeScene(GetGame()->GetPlayScene());
        }
        switch (m_menu->GetActiveUI())
        {
        case 0:
            //GetGame()->ChangeScene(GetGame()->GetPlayScene());
            m_isStageSelect = true;
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
        }
    }

}

void TitleScene::Render()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_skydome->Render();
    RenderManager::RenderObjects();
}

void TitleScene::RenderUI()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_titleLogo->Render(Vector2::Zero,Colors::White,Vector2::Zero,{0.5f,0.5f});
    m_menu->Render(Vector2{ 1500 - m_menuPosition * 1280,0});

    if (m_isStageSelect)
    {
        m_menuBack->Render(Vector2{ 1920 - m_stageMenuPosition * 1280,360 }, DirectX::XMVECTORF32({ 1,1,1,0.5f }), Vector2(640, 360), Vector2(0.9f, 0.9f));
        m_stageMenu->Render(Vector2{ 1920 - m_stageMenuPosition * 1280,0   });
    }
}

void TitleScene::Finalize()
{
    RenderManager::Clear();
}
