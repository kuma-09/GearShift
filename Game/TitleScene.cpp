#include "pch.h"
#include "TitleScene.h"
#include "Game.h"
#include "Game/Components/Camera.h" 
#include "Framework/Resources.h"
#include "Framework/BinaryFile.h"

void TitleScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    SetGame(game);
 


    m_titleLogo = std::make_unique<UI>(L"Resources/Textures/GearShiftLogo.png", Vector2{ 0,0 });
    m_titleLogo->Initialize();

    m_skydome = std::make_unique<SkyDome>();
    m_skydome->Initialize({ 0,-20,0 });
    
    m_player = std::make_unique<TitlePlayer>(this);
    m_player->SetPosition(Vector3{ 0,2.4,0 });
    m_player->Initialize();

    
    m_camera = std::make_unique<TitleCamera>();
    m_camera->Initialize(m_player.get());
    m_camera->SetPosition(Vector3{ 0,5,-5 });


}


void TitleScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;
    
    const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    m_skydome->Update(elapsedTime);
    m_camera->Update(elapsedTime);
    m_player->Update(elapsedTime);

    if (kb->pressed.Space || gp->a)
    {
        GetGame()->ChangeScene(GetGame()->GetPlayScene());
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
    m_titleLogo->Render();

}

void TitleScene::Finalize()
{

}
