#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();

    //// ビュー行列を作成
    //Vector3 eye = Vector3::Zero;
    //Vector3 target = Vector3(0,0,5);
    //Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    //m_graphics->SetViewMatrix(view);

    //// 射影行列を作成する
    //Matrix projection = Matrix::CreatePerspectiveFieldOfView(
    //    XMConvertToRadians(45.0f),
    //    static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().right) / static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().bottom),
    //    0.1f, 1000.0f);
    //m_graphics->SetProjectionMatrix(projection);

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

    // プレイヤーの向いている方向を計算
    Vector3 dot = GetOwner()->GetPosition() - m_targetPosition;
    float radianY = atan2f(dot.x, dot.z);

    // プレイヤーの向いている方向を設定
    Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(0, radianY, 0));
    GetOwner()->SetQuaternion(quaternion);

    // カメラの座標を更新
    Vector3 pos{ 0,CAMERA_HEIGHT,CAMERA_DISTANCE };
    pos = Vector3::Transform(pos, GetOwner()->GetQuaternion());
    m_eyePosition += ((m_player->GetPosition() + pos) - m_eyePosition) * CAMERA_EYE_RATE;

    // ターゲットの座標を更新
    m_targetPosition = Vector3::Lerp(m_targetPosition, m_enemy->GetPosition(), CAMERA_TARGET_RATE);

    // View行列の更新
    Matrix view = Matrix::CreateLookAt(m_eyePosition, m_targetPosition, Vector3::UnitY);
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