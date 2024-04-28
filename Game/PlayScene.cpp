#include "pch.h"
#include "PlayScene.h"

void PlayScene::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    m_model = std::make_unique<DirectX::Model>();
    m_model = DirectX::Model::CreateFromCMO(m_graphics->GetDeviceResources()->GetD3DDevice(), L"Resources/Models/dice.cmo", *m_graphics->GetFX());



    // �r���[�s����쐬
    Vector3 eye = Vector3(0, 5, 10);
    Vector3 target = Vector3::Zero;
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);

    // �ˉe�s����쐬����
    Matrix projection = Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(45.0f),
        static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().right) / static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().bottom),
        0.1f, 100.0f
    );
    m_graphics->SetProjectionMatrix(projection);

    m_angle = 0;
    m_position = Vector3::Zero;
}

void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    const auto& kbState = m_inputManager->GetKeyboardState();
    const auto& kb = m_inputManager->GetKeyboardTracker();
    const auto& gp = m_inputManager->GetGamePadTracker();



    // �u�C�������v�̉�]�s��𐶐�����
    //turretRotation = Matrix::CreateRotationY(GetInitialAngleRL() + GetAngle());
    Matrix rotation = Matrix::CreateRotationY(m_angle);
    // �u�C�������v�̑��x���v�Z����
    //turretVelocity = SPEED.z * turretRotation.Forward();
    Vector3 velocity = 0.05f * rotation.Forward();
    // ���C����O�i����
    //m_currentPosition -= turretVelocity;

    if (gp->dpadDown)
    {
        m_position += velocity;
    }
}

void PlayScene::Render()
{
    using namespace DirectX::SimpleMath;


    auto context = m_deviceResources->GetD3DDeviceContext();

    m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    Matrix world = Matrix::Identity;
    world = Matrix::CreateTranslation(m_position);


    m_model->Draw(context, *m_graphics->GetCommonStates(), world, m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    m_graphics->DrawPrimitiveEnd();
}

void PlayScene::Finalize()
{

}