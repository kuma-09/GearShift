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

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    SetGame(game);

    m_titleLogo = std::make_unique<UI>(L"Resources/Textures/GearShiftLogo.png", Vector2{ 0,0 });
    m_titleLogo->Initialize();

    m_skydome = std::make_unique<SkyDome>();
    m_skydome->Initialize({ 0,-20,0 });
    
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

    m_shadow = std::make_unique<Shadow>();
    m_shadow->Initialize();

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
    m_shadow->Update();
}

void TitleScene::Render()
{








	////	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	//context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuffer, 0, 0);

	////	シェーダーにバッファを渡す
	//ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	////	頂点シェーダもピクセルシェーダも、同じ値を渡す
	//context->VSSetConstantBuffers(0, 1, cb);
	//context->PSSetConstantBuffers(0, 1, cb);

    m_shadow->BeginDepth();
    m_shadow->RenderDepth({0,1,0});
    m_shadow->RenderDepth({ 3,1,3 });
    m_shadow->EndDepth();

    m_shadow->Render({ 0,1,0 });
    m_shadow->Render({ 3,1,3 });
    m_shadow->End();

    m_skydome->Render();
    //m_player->Render();
    //for (auto& position : m_floorPosition)
    //{
    //    for (int i = 0; i < 2; i++)
    //    {
    //        for (int n = 0; n < 2; n++)
    //        {
    //            Matrix world = Matrix::CreateTranslation(Vector3{ -50 + float(i) * 100 ,1, -50 + float(n) * 100 } + position);
    //            m_floorModel->Draw(context, *state, world, view, proj, false);
    //        }
    //    }
    //}




    m_titleLogo->Render();





}

void TitleScene::Finalize()
{

}
