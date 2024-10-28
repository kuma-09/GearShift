#include "pch.h"
#include "Move.h"
#include "Game/Player/Player.h"
#include "Game/GameObject.h"
#include "Game/Components/Camera.h"

Move::Move()
{
	m_inputManager = InputManager::GetInstance();

}

Move::~Move()
{

}

void Move::Initialize()
{

}

void Move::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    const auto& kb = m_inputManager->GetKeyboardState();
    const auto& gpState = m_inputManager->GetGamePadState();

    // パッドの入力情報
    Vector3 input = Vector3{ gpState.thumbSticks.leftX,0, -gpState.thumbSticks.leftY } * elapsedTime;

    // 親オブジェクトの向いている方向
    Quaternion quaternion = GetOwner()->GetQuaternion();
    quaternion = Quaternion::CreateFromYawPitchRoll(GetOwner()->GetQuaternion().ToEuler().y, 0, 0);
    quaternion = Quaternion::CreateFromYawPitchRoll(GetOwner()->GetComponent<Camera>()->GetCameraQuaternion().ToEuler().y, 0, 0);

    // 親オブジェクトに渡すベクトル
    if (gpState.thumbSticks.leftY != 0)
    {
        m_velocity += input;
    }
    if (gpState.thumbSticks.leftX != 0)
    {
        m_velocity += input;
    }

    if (kb.W)
    {
        m_velocity += Vector3::Forward * 0.025f;
    }
    if (kb.S)
    {
        m_velocity += Vector3::Backward * 0.025f;
    }
    if (kb.A)
    {
        m_velocity += Vector3::Left * 0.025f;
    }
    if (kb.D)
    {
        m_velocity += Vector3::Right * 0.025;
    };
    m_velocity = Vector3::Lerp(m_velocity, Vector3::Zero, elapsedTime);

    //m_velocity.Normalize();
    GetOwner()->SetVelocity(Vector3::Transform(m_velocity, quaternion));

    if (!static_cast<Player*>(GetOwner())->GetTarget() && m_velocity != Vector3::Zero)
    {
        Vector3 velocity = GetOwner()->GetVelocity();
        velocity.x *= -1;
        quaternion = Quaternion::CreateFromRotationMatrix(Matrix::CreateLookAt(GetOwner()->GetPosition(), GetOwner()->GetPosition() + Vector3::Transform(Vector3::Forward,GetOwner()->GetQuaternion()), Vector3(0, 1, 0)));
        GetOwner()->SetQuaternion(Quaternion::Lerp(GetOwner()->GetQuaternion(), quaternion,0.1f));
    }

}

void Move::Finalize()
{

}

DirectX::SimpleMath::Vector3 Move::GetVelocity()
{
    return m_velocity;
}
