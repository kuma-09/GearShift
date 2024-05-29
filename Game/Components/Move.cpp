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

    if (gpState.thumbSticks.leftY != 0)
    {
        velocity += 5.0f * Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Forward() * elapsedTime * gpState.thumbSticks.leftY;
    }
    if (gpState.thumbSticks.leftX != 0)
    {
        velocity += 5.0f * Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * elapsedTime * gpState.thumbSticks.leftX;
    }

    velocity.Normalize();
    GetOwner()->SetPosition(GetOwner()->GetPosition() + velocity / 5);
}


void Move::Finalize()
{

}