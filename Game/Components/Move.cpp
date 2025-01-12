#include "pch.h"
#include "Move.h"
#include "Game/Player/Player.h"
#include "Game/GameObject.h"
#include "Game/Components/InputComponent.h"

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



    // ƒpƒbƒh‚Ì“ü—Íî•ñ
    Vector3 input = GetOwner()->GetComponent<InputComponent>()->GetVelocity();
    
    bool isMove = false;
    if (input != Vector3::Zero)
    {
        isMove = true;
        m_velocity += input;
    }

    m_isMove = isMove;
    m_velocity = Vector3::Lerp(m_velocity, Vector3::Zero, 0.1f) * MAX_SPEED;

}

DirectX::SimpleMath::Vector3 Move::GetVelocity()
{
    return m_velocity;
}
