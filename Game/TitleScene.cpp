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
    m_menu->AddUI(L"Resources/Textures/Stage1.png", {0,150});
    m_menu->AddUI(L"Resources/Textures/Stage2.png", {0,300});
    m_menu->AddUI(L"Resources/Textures/Stage3.png", {0,450});
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

void TitleScene::Render()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto state = m_graphics->GetCommonStates();
    auto view = m_graphics->GetViewMatrix();
    auto proj = m_graphics->GetProjectionMatrix();



    Resources::GetInstance()->GetShadow()->BeginDepth();
    m_player->CreateShadow();
    Resources::GetInstance()->GetShadow()->EndDepth();

    m_skydome->Render();
    m_player->Render();
    for (int i = 0; i < 2; i++)
    {
        for (int n = 0; n < 2; n++)
        {
            Matrix world = Matrix::CreateTranslation(Vector3{ -50 + float(i) * 100 ,1, -50 + float(n) * 100 } + Vector3::Zero );
            Resources::GetInstance()->GetFloorModel()->Draw(context, *state, world, view, proj, false, [&]
                {
                    Resources::GetInstance()->GetShadow()->Draw(true);
                }
            );
        }
    }


    Resources::GetInstance()->GetShadow()->End();
    m_titleLogo->Render(Vector2::Zero,Colors::White,Vector2::Zero,{0.5f,0.5f});
    if (m_isStageSelect)
    {
        m_menuBack->Render({ m_menuPosition * 1280 - 640,720 / 2 }, {1,1,1,0.5f}, Vector2{1280,720} / 2, {0.95f,0.95f});
    }
    m_menu->Render(Vector2{m_menuPosition * 1280 - 640,0});

}

void TitleScene::Finalize()
{

}
