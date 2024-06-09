#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();

    // �r���[�s����쐬
    Vector3 eye = Vector3::Zero;
    Vector3 target = Vector3(0,0,5);
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);

    // �ˉe�s����쐬����
    Matrix projection = Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(45.0f),
        static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().right) / static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().bottom),
        0.1f, 1000.0f);
    m_graphics->SetProjectionMatrix(projection);

}

Camera::~Camera()
{

}

void Camera::Initialize()
{

}

void Camera::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    UNREFERENCED_PARAMETER(elapsedTime);

    // �^�[�Q�b�g�̍��W�Ɍ������Ď��_���ړ�����
    m_targetpos = Vector3::Lerp(m_targetpos, m_enemy->GetPosition(), 0.1f);

    // �v���C���[�̌����Ă���������v�Z
    Vector3 dot = GetOwner()->GetPosition() - m_targetpos;
    float radianY = atan2f(dot.x, dot.z);
    float radianX = atan2f(dot.y, dot.z);

    // �v���C���[�̌����Ă��������ݒ�
    Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(0, radianY, 0));
    GetOwner()->SetQuaternion(quaternion);

    // View�s��̍X�V
    Vector3 eye = m_player->GetPosition() + 15 * -Matrix::CreateFromQuaternion(quaternion).Forward() + 5 * Matrix::CreateFromQuaternion(quaternion).Up();
    Matrix view = Matrix::CreateLookAt(eye, m_targetpos, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);
}

void Camera::Render()
{

}

void Camera::Finalize()
{

}

void Camera::SetTarget(GameObject* player, GameObject* enemy)
{
    m_player = player;
    m_enemy  = enemy;
}