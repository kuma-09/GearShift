#include "pch.h"
#include "TestScene.h"
#include "Framework/BinaryFile.h"

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

TestScene::TestScene()
{
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();


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


    //	インプットレイアウトの作成
    device->CreateInputLayout(&INPUT_LAYOUT[0],
        static_cast<UINT>(INPUT_LAYOUT.size()),
        vs.GetData(), vs.GetSize(),
        m_inputLayout.GetAddressOf());

    m_effect = std::make_unique<HitEffect>();
    m_effect->Set(Vector3::Zero);
    m_time = 0;
}


void TestScene::Update(float elapsedTime)
{
    m_time+= elapsedTime;
    if (m_time >= 0.5f)
    {
        m_effect->Set(DirectX::SimpleMath::Vector3(rand() % 2, rand() % 2, rand() % 2));
        m_time = 0;
    }

    m_effect->Update(elapsedTime);

}

void TestScene::Render()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto state = m_graphics->GetCommonStates();

    auto view = m_graphics->GetViewMatrix();
    auto proj = m_graphics->GetProjectionMatrix();

    Resources::GetInstance()->GetCubeModel()->Draw(context, *state, Matrix::Identity, view, proj, false, [&]
        {
            context->IASetInputLayout(m_inputLayout.Get());
            context->VSSetShader(m_vs.Get(), nullptr, 0);
            context->PSSetShader(m_ps.Get(), nullptr, 0);
        });

    m_effect->Render(view, proj);

}

void TestScene::Finalize()
{

}
