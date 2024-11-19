#include "pch.h"
#include "TestScene.h"
#include "Framework/BinaryFile.h"

TestScene::TestScene()
{
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();
}

void TestScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    SetGame(game);

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();

    Matrix view = Matrix::CreateLookAt(Vector3(3, 5, -10),
        Vector3::Zero, Vector3::Up);

    m_graphics->SetViewMatrix(view);


    BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/BurnerVS.cso");
    BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/BurnerPS.cso");

    // 頂点シェーダーの作成
    DX::ThrowIfFailed(
        device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_vs.ReleaseAndGetAddressOf())
    );
    // ピクセルシェーダーの作成
    DX::ThrowIfFailed(
        device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_ps.ReleaseAndGetAddressOf())
    );

    m_time = 0;
}


void TestScene::Update(float elapsedTime)
{
    m_time+= elapsedTime;


}

void TestScene::Render()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

}

void TestScene::Finalize()
{

}
