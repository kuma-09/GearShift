#include "pch.h"
#include "Camera.h"
#include "math.h"

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

    if (m_rotateY >= 1.f)
    {
        m_rotateY = 1.f;
    }
    else if(m_rotateY <= -1.f)
    {
        m_rotateY = -1.f;
    }

    m_quaternion = Quaternion::CreateFromYawPitchRoll({ m_rotateY ,m_rotateX ,0 });

    m_targetPosition += (m_player->GetPosition() - m_targetPosition);


    // �J�����̃f�t�H���g�̍��W�x�N�g��
    DirectX::SimpleMath::Vector3 eye{ 0.0f,CAMERA_HEIGHT,CAMERA_DISTANCE };

    // �^�[�Q�b�g�̌����Ă�������ɒǏ]����
    eye = DirectX::SimpleMath::Vector3::Transform(eye, m_quaternion);

    // �J�������W���v�Z����
    m_eyePosition += (m_targetPosition + eye - m_eyePosition);

    m_shakeTime -= elapsedTime;
    if (m_shakeTime >= 0)
    {

        if (m_shakeRate > 0)
        {
            m_shakeRate -= elapsedTime;
            m_shakeRate *= -1.0f;
        }
        else if (m_shakeRate < 0)
        {
            m_shakeRate += elapsedTime;
            m_shakeRate *= -1.0f;
        }
        Vector3 velocity = Vector3::Transform(Vector3::Lerp( Vector3::Zero, { m_shakeRate,m_shakeRate,0 }, m_shakeTime), GetOwner()->GetQuaternion());
        m_targetPosition += velocity;
    }


    
    //GetOwner()->SetQuaternion(Quaternion::CreateFromYawPitchRoll({ 0 ,m_rotateX ,0 }));

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
    m_shakeTime = SHAKE_TIME;
    Vector3 velocity = Vector3::Transform({m_shakeRate,m_shakeRate,m_shakeRate}, GetOwner()->GetQuaternion());
    m_targetPosition += velocity;
}

void Camera::SetTarget(GameObject* player, GameObject* enemy)
{
    m_player = player;
    m_enemy  = enemy;
}


