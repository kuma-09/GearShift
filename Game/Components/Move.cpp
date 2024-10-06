#include "pch.h"
#include "Move.h"
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
    Vector3 velocity = Vector3::Zero;

    if (gpState.thumbSticks.leftY != 0)
    {
        velocity += Vector3::Transform(input,quaternion);
    }
    if (gpState.thumbSticks.leftX != 0)
    {
        velocity += Vector3::Transform(input, quaternion);
    }

    if (kb.W)
    {
        velocity += Vector3::Transform(Vector3::Forward * elapsedTime, quaternion);
    }
    if (kb.S)
    {
        velocity += Vector3::Transform(Vector3::Backward * elapsedTime, quaternion);
    }
    if (kb.A)
    {
        velocity += Vector3::Transform(Vector3::Left * elapsedTime, quaternion);
    }
    if (kb.D)
    {
        velocity += Vector3::Transform(Vector3::Right * elapsedTime, quaternion);
    }
    ;


    velocity.Normalize();
    GetOwner()->SetVelocity(velocity / 3);

    if (velocity != Vector3::Zero)
    {
        velocity.x *= -1;
        quaternion = Quaternion::CreateFromRotationMatrix(Matrix::CreateLookAt(GetOwner()->GetPosition(), GetOwner()->GetPosition() + velocity, Vector3(0,1,0)));
        GetOwner()->SetQuaternion(Quaternion::Lerp(GetOwner()->GetQuaternion(), quaternion,0.1f));
    }

}


void Move::Finalize()
{

}