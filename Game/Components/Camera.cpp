#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
    m_targetPosition = DirectX::SimpleMath::Vector3::One;
    m_eyePosition = DirectX::SimpleMath::Vector3::Zero;
    m_quaternion = DirectX::SimpleMath::Quaternion::Identity;

    m_graphics = Graphics::GetInstance();
    m_shakeTime = 0;
    m_rotateX = 0;
    m_rotateY = 0;
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
    
    auto mouse = InputManager::GetInstance()->GetMouseState();

    m_rotateX -= mouse.x * 0.001f;
    m_rotateY -= mouse.y * 0.001f;


    m_quaternion = Quaternion::CreateFromYawPitchRoll({ m_rotateY ,m_rotateX ,0 });

    m_targetPosition += (m_player->GetPosition() - m_targetPosition);	// バネ付き

    // カメラのデフォルトの座標ベクトル
    DirectX::SimpleMath::Vector3 eye{ 0.0f,1,20 };

    // ターゲットの向いている方向に追従する
    eye = DirectX::SimpleMath::Vector3::Transform(eye, m_quaternion);

    // カメラ座標を計算する
    //m_eye = m_target + eye;	// バネなし
    m_eyePosition += (m_targetPosition + eye - m_eyePosition) ;	// バネ付き


    //// カメラの座標を更新
    //Vector3 pos{ 0,CAMERA_HEIGHT,CAMERA_DISTANCE };
    //pos = Vector3::Transform(pos, GetOwner()->GetQuaternion());
    //m_eyePosition += ((m_player->GetPosition() + pos) - m_eyePosition) * CAMERA_EYE_RATE;

    //// ターゲットの座標を更新
    //m_quaternion += Quaternion::CreateFromYawPitchRoll({ x ,y,0 });
    //Vector3 target;
    //target = Vector3::Transform(m_targetPosition, m_quaternion);
    //m_targetPosition += ((m_player->GetPosition() + pos) - m_eyePosition) * CAMERA_TARGET_RATE;
    ////m_targetPosition = Vector3::Lerp(m_targetPosition, m_enemy->GetPosition(), CAMERA_TARGET_RATE);
    
    GetOwner()->SetQuaternion(m_quaternion);

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

void Camera::shake()
{
    //using namespace DirectX::SimpleMath;
    //m_shakeRate = SHAKE_RATE;
    //Vector3 velocity = Vector3::Transform({-1,-1,-1}, GetOwner()->GetQuaternion());
    //m_targetPosition += velocity;
}

void Camera::SetTarget(GameObject* player, GameObject* enemy)
{
    m_player = player;
    m_enemy  = enemy;
}