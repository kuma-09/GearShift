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

	UNREFERENCED_PARAMETER(elapsedTime);

    const auto& kb = m_inputManager->GetKeyboardState();
    const auto& gpState = m_inputManager->GetGamePadState();
    const auto& gpTracker = m_inputManager->GetGamePadTracker();


    Vector3 velocity = Vector3::Zero;

    Matrix rotate = Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());

    if (gpState.thumbSticks.leftY != 0)
    {
        velocity +=  rotate.Forward() * elapsedTime * gpState.thumbSticks.leftY;
    }
    if (gpState.thumbSticks.leftX != 0)
    {

        velocity +=  rotate.Right() * elapsedTime * gpState.thumbSticks.leftX;
    }

    if (kb.Up)
    {
        velocity += rotate.Forward() * elapsedTime ;
    }
    if (kb.Down)
    {
        velocity +=  -rotate.Forward() * elapsedTime ;
    }
    if (kb.Left)
    {
        velocity +=  -rotate.Right() * elapsedTime ;
    }
    if (kb.Right)
    {
        velocity +=  rotate.Right() * elapsedTime ;
    }

    velocity.Normalize();
    GetOwner()->SetVelocity(velocity / 5);
}


void Move::Finalize()
{

}