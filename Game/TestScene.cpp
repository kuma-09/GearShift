#include "pch.h"
#include "TestScene.h"


TestScene::TestScene()
{
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    m_shadowMap = std::make_unique<Imase::ShadowMap>();
    m_shadowMap->Initialize(device,L"Resources/Shaders", 2048);

    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

}

void TestScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    SetGame(game);

    Matrix view = Matrix::CreateLookAt(Vector3(0, 5, -5),
        Vector3::Zero, Vector3::Up);

    m_graphics->SetViewMatrix(view);

    Matrix proj = Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(45.0f), 1280 / 720, 0.1f, 100.0f);

    m_graphics->SetProjectionMatrix(proj);



}


void TestScene::Update(float elapsedTime)
{
    UNREFERENCED_PARAMETER(elapsedTime);

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

    m_shadowMap->Begin(context, { 0,5,-5 }, Vector3::Zero, 100);



    Resources::GetInstance()->GetCubeModel()->Draw(context, *state, Matrix::Identity, view, proj, false, [&]()
        {
            m_shadowMap->DrawShadowMap(context);
        });


    m_spriteBatch->Begin();
    m_spriteBatch->Draw(m_shadowMap->GetShadowMapTexture(), Vector2::Zero);
    m_spriteBatch->End();
}

void TestScene::Finalize()
{

}
