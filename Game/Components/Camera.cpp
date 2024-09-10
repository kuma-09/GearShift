#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
    m_graphics = Graphics::GetInstance();
    m_shakeTime = 0;
}

Camera::~Camera()
{

}

void Camera::Initialize()
{
    m_targetPosition = DirectX::SimpleMath::Vector3::One;
}

void Camera::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    UNREFERENCED_PARAMETER(elapsedTime);
    
    auto mouse = InputManager::GetInstance()->GetMouseState();
    auto& mouseT = InputManager::GetInstance()->GetMouseTracker();

    float x = mouse.x;
    float y = mouse.y;

    // �J�����̍��W���X�V
    Vector3 pos{ 0,CAMERA_HEIGHT,CAMERA_DISTANCE };
    pos = Vector3::Transform(pos, GetOwner()->GetQuaternion());
    m_eyePosition += ((m_player->GetPosition() + pos) - m_eyePosition) * CAMERA_EYE_RATE;

    // �^�[�Q�b�g�̍��W���X�V
    //m_targetPosition = Vector3::Lerp(m_targetPosition, m_enemy->GetPosition(), CAMERA_TARGET_RATE);

    // View�s��̍X�V
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
    using namespace DirectX::SimpleMath;
    m_shakeRate = SHAKE_RATE;
    Vector3 velocity = Vector3::Transform({-1,-1,-1}, GetOwner()->GetQuaternion());
    m_targetPosition += velocity;
}

void Camera::SetTarget(GameObject* player, GameObject* enemy)
{
    m_player = player;
    m_enemy  = enemy;
}