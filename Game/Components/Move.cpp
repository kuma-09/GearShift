#include "pch.h"
#include "Move.h"
#include "Game/Player/Player.h"
#include "Game/GameObject.h"
#include "Game/Components/Camera.h"

Move::Move()
{
}

Move::~Move()
{
}

void Move::Initialize()
{
    m_inputManager = InputManager::GetInstance();
    m_velocity = DirectX::SimpleMath::Vector3::Zero;
    m_isMove = false;   
}

void Move::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    const auto& kb = m_inputManager->GetKeyboardState();
    const auto& gpState = m_inputManager->GetGamePadState();

    // ÉpÉbÉhÇÃì¸óÕèÓïÒ
    Vector3 input = Vector3{ gpState.thumbSticks.leftX,0, -gpState.thumbSticks.leftY };
    input.Normalize();
    input *= 0.05f;
    
    bool isMove = false;
    if (input != Vector3::Zero)
    {
        m_velocity += input;
        isMove = true;
    }

    if (kb.W)
    {
        m_velocity += Vector3::Forward * 0.05f;
        isMove = true;
    }
    if (kb.S)
    {
        m_velocity += Vector3::Backward * 0.05f;
        isMove = true;
    }
    if (kb.A)
    {
        m_velocity += Vector3::Left * 0.05f;
        isMove = true;
    }
    if (kb.D)
    {
        m_velocity += Vector3::Right * 0.05f;
        isMove = true;
    };
    m_isMove = isMove;
    m_velocity = Vector3::Lerp(m_velocity, Vector3::Zero, 0.1f) * MAX_SPEED;

}

DirectX::SimpleMath::Vector3 Move::GetVelocity()
{
    return m_velocity;
}
