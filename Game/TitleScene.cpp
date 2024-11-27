#include "pch.h"
#include "TitleScene.h"
#include "Game.h"
#include "Game/Components/Camera.h" 
#include "Framework/Resources.h"
#include "Framework/BinaryFile.h"
#include "Framework/Easing.h"

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

    m_menu = std::make_unique<Menu>();
    m_menu->AddUI(L"Resources/Textures/Start.png", {0,450}, {0.5f,0.5f});
    m_menu->AddUI(L"Resources/Textures/Option.png",{0,550}, {0.5f,0.5f});
    m_menu->AddUI(L"Resources/Textures/Exit.png",  {0,650}, {0.5f,0.5f});
    m_menu->Initialize();
    m_menuBack = std::make_unique<UI>(L"Resources/Textures/SceneChangeBlack.png");
    m_menuBack->Initialize();
    m_nowTime = 0;
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
    m_menu->Update();
    m_menuPosition = Easing::InOutQuart(m_nowTime, 1.0f);
    if (kb->pressed.Space || gp->a)
    {
        switch (m_menu->GetActiveUI())
        {
        case 0:
            GetGame()->ChangeScene(GetGame()->GetPlayScene());
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

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto state = m_graphics->GetCommonStates();
    auto view = m_graphics->GetViewMatrix();
    auto proj = m_graphics->GetProjectionMatrix();



    //Resources::GetInstance()->GetShadow()->BeginDepth();
    //m_player->CreateShadow();
    //Resources::GetInstance()->GetShadow()->EndDepth();

    m_skydome->Render();
    m_player->Render();
    //for (int i = 0; i < 2; i++)
    //{
    //    for (int n = 0; n < 2; n++)
    //    {
    //        Matrix world = Matrix::CreateTranslation(Vector3{ -50 + float(i) * 100 ,1, -50 + float(n) * 100 } + Vector3::Zero );
    //        Resources::GetInstance()->GetFloorModel()->Draw(context, *state, world, view, proj, false, [&]
    //            {
    //                Resources::GetInstance()->GetShadow()->Draw(true);
    //            }
    //        );
    //    }
    //}


    //Resources::GetInstance()->GetShadow()->End();
    m_titleLogo->Render(Vector2::Zero,Colors::White,Vector2::Zero,{0.5f,0.5f});
    m_menu->Render(Vector2{ 1500 - m_menuPosition * 1280,0});

}

void TitleScene::Finalize()
{

}
