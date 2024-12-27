#include "pch.h"
#include "Camera.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"

Camera::Camera()
{
	m_graphics = Graphics::GetInstance();
    m_inputManager = InputManager::GetInstance();
	m_rotateX = 0;
	m_rotateY = 0;
	m_target = nullptr;
	m_targetPosition = DirectX::SimpleMath::Vector3::One;
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

    const auto& mouse = m_inputManager->GetMouseState();
    const auto& gpState = m_inputManager->GetGamePadState();

    // パッドの入力情報
    Vector2 input = Vector2(gpState.thumbSticks.rightX, -gpState.thumbSticks.rightY) * elapsedTime;

    m_rotateX -= mouse.x * 0.001f;
    m_rotateY -= mouse.y * 0.001f;
    m_rotateX -= input.x * 2;
    m_rotateY -= input.y * 2;

    if (m_rotateY >= 1.f)
    {
        m_rotateY = 1.f;
    }
    else if (m_rotateY <= -1.f)
    {
        m_rotateY = -1.f;
    }

    if (m_target)
    {
        SetQuaternion(Quaternion::Lerp(GetQuaternion(), Quaternion::CreateFromRotationMatrix(Matrix::CreateLookAt(GetPosition(), m_targetPosition, Vector3::Down)), 0.25f));
        Vector3 rotate = GetQuaternion().ToEuler();
        m_rotateX = rotate.y;
        m_rotateY = rotate.x;
        // カメラのデフォルトの座標ベクトル
        DirectX::SimpleMath::Vector3 eye{ 0.0f,-CAMERA_HEIGHT,CAMERA_DISTANCE };
        // ターゲットの向いている方向に追従する
        eye = DirectX::SimpleMath::Vector3::Transform(eye, GetQuaternion());
        m_targetPosition += (m_target->GetPosition() - m_targetPosition) * CAMERA_TARGET_RATE;
        SetPosition(GetPosition() + (m_player->GetPosition() + eye - GetPosition()) * (CAMERA_EYE_RATE * 0.25f));
    }
    else
    {
        SetQuaternion(Quaternion::CreateFromYawPitchRoll({ m_rotateY ,m_rotateX ,0 }));
        // カメラのデフォルトの座標ベクトル
        DirectX::SimpleMath::Vector3 eye{ 0.0f,CAMERA_HEIGHT,CAMERA_DISTANCE };
        // ターゲットの向いている方向に追従する
        eye = DirectX::SimpleMath::Vector3::Transform(eye, GetQuaternion());
        m_targetPosition += (m_player->GetPosition() - m_targetPosition) * CAMERA_TARGET_RATE;
        // カメラ座標を計算する
        SetPosition(GetPosition() + ((m_targetPosition + eye - GetPosition()) * CAMERA_EYE_RATE));
    }

    // View行列の更新
    Matrix view = Matrix::CreateLookAt(GetPosition(), m_targetPosition, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);
    
}

void Camera::SetTarget(GameObject* player, GameObject* target)
{
    m_player = player;
    m_target = target;
}
