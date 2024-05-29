#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();

    // ビュー行列を作成
    Vector3 eye = Vector3::Zero;
    Vector3 target = Vector3(0,0,5);
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);

    // 射影行列を作成する
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
    
    Vector3 dot = GetOwner()->GetPosition() - m_enemy->GetPosition();
    float radian = atan2f(dot.x, dot.z);

    Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(0, radian, 0));

    GetOwner()->SetQuaternion(quaternion);

    // ビュー行列を作成
    Vector3 eye = m_player->GetPosition() + 15 * -Matrix::CreateFromQuaternion(quaternion).Forward() + 5 * Matrix::CreateFromQuaternion(quaternion).Up();
    Vector3 target = m_enemy->GetPosition();
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
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