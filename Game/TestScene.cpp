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

    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
    m_shadow = std::make_unique<Shadow>();

}

void TestScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    SetGame(game);

    Matrix view = Matrix::CreateLookAt(Vector3(3, 5, -10),
        Vector3::Zero, Vector3::Up);

    m_graphics->SetViewMatrix(view);

    m_shadow->Initialize();

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

    m_shadow->BeginDepth();

    Resources::GetInstance()->GetCubeModel()->Draw(context, *state, Matrix::Identity, view, proj, false, [&]()
        {
            m_shadow->RenderDepth();
        }
    );

    Resources::GetInstance()->GetCubeModel()->Draw(context, *state, Matrix::CreateTranslation({0,0,3}), view, proj, false, [&]()
        {
            m_shadow->RenderDepth();
        }
    );


    m_shadow->EndDepth();

    


    Resources::GetInstance()->GetCubeModel()->Draw(context, *state, Matrix::CreateTranslation({0,0,2}), view, proj, false, [&]()
        {
            m_shadow->Draw(false);
        }
    );
    m_shadow->End();

}

void TestScene::Finalize()
{

}
