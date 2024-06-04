#include "pch.h"
#include "MoveRotation.h"
#include "Game/GameObject.h"

MoveRotation::MoveRotation()
{
	m_inputManager = InputManager::GetInstance();
}

MoveRotation::~MoveRotation()
{

}

void MoveRotation::Initialize()
{

}

void MoveRotation::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

	UNREFERENCED_PARAMETER(elapsedTime);

    const auto& kb = m_inputManager->GetKeyboardState();
    const auto& gpState = m_inputManager->GetGamePadState();
    const auto& gpTracker = m_inputManager->GetGamePadTracker();

    if (gpState.thumbSticks.leftY != 0)
    {

    }
    if (gpState.thumbSticks.leftX != 0)
    {

    }

    if (kb.Up)
    {

    }
    if (kb.Down)
    {

    }
    if (kb.Left)
    {

    }
    if (kb.Right)
    {

    }
}


void MoveRotation::Finalize()
{

}