#include "pch.h"
#include "Move.h"
#include "Game/GameObject.h"

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
    const auto& gpTracker = m_inputManager->GetGamePadTracker();

    // パッドの入力情報
    Vector3 input = Vector3{ gpState.thumbSticks.leftX,0, -gpState.thumbSticks.leftY } * elapsedTime;

    // 親オブジェクトの向いている方向
    Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(0, GetOwner()->GetQuaternion().ToEuler().y,0);

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

    if (kb.Up)
    {
        velocity += Vector3::Transform(Vector3(0, 0, -1) * elapsedTime, quaternion);
    }
    if (kb.Down)
    {
        velocity += Vector3::Transform(Vector3(0, 0,  1) * elapsedTime, quaternion);
    }
    if (kb.Left)
    {
        velocity += Vector3::Transform(Vector3(-1, 0, 0) * elapsedTime, quaternion);
    }
    if (kb.Right)
    {
        velocity += Vector3::Transform(Vector3( 1, 0, 0) * elapsedTime, quaternion);
    }

    velocity.Normalize();
    GetOwner()->SetVelocity(velocity / 5);
}


void Move::Finalize()
{

}