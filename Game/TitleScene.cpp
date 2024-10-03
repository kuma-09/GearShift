#include "pch.h"
#include "TitleScene.h"
#include "Game.h"
#include "Game/Components/Camera.h" 
#include "Framework/Resources.h"


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
    m_titleLogo->Create(m_deviceResources, L"Resources/Textures/GearShiftLogo.png", Vector2(x / 2.0f,y ), Vector2::One * 2, tito::MIDDLE_CENTER);
    m_titleLogo->SetWindowSize(x, y);
    
    m_skydome = std::make_unique<SkyDome>();
    m_player = std::make_unique<TitlePlayer>(this);
    m_player->Initialize();
    
    m_camera = std::make_unique<TitleCamera>();
    m_camera->Initialize(m_player.get());
    m_camera->SetPosition(Vector3{ 0,5,-20 });
    
    m_floorModel = Resources::GetInstance()->GetFloorModel();

    m_floorPosition.push_back({ -100,0,-100 });
    m_floorPosition.push_back({  100,0,-100 });
    m_floorPosition.push_back({ -100,0, 100 });
    m_floorPosition.push_back({  100,0, 100 });

}


void TitleScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    
    //const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    m_skydome->Update(elapsedTime);
    m_camera->Update(elapsedTime);

    m_player->Update(elapsedTime);

    if (kb->pressed.Space)
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


    m_skydome->Render();
    m_titleLogo->Render();



    for (auto& position : m_floorPosition)
    {
        for (int i = 0; i < 2; i++)
        {
            for (int n = 0; n < 2; n++)
            {
                Matrix world = Matrix::CreateTranslation(Vector3{ -50 + float(i) * 100 ,1, -50 + float(n) * 100 } + position);
                m_floorModel->Draw(context, *state, world, view, proj);
            }
        }
    }

    m_player->Render();

}

void TitleScene::Finalize()
{

}
