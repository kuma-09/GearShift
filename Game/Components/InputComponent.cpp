#include "pch.h"
#include "InputComponent.h"

InputComponent::InputComponent()
{
    m_inputManager = InputManager::GetInstance();
}

InputComponent::~InputComponent()
{
}

void InputComponent::Initialize()
{
}

void InputComponent::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    const auto& kb = m_inputManager->GetKeyboardState();
    const auto& gpState = m_inputManager->GetGamePadState();

    // ƒpƒbƒh‚Ì“ü—Íî•ñ
    Vector3 input = Vector3{ gpState.thumbSticks.leftX,0, -gpState.thumbSticks.leftY };
    if (kb.W)
    {
        input.z = 1;
    }
    if (kb.S)
    {
        input.z = -1;
    }
    if (kb.A)
    {
        input.x = -1;
    }
    if (kb.D)
    {
        input.x = 1;
    };



    input.Normalize();
    input *= 0.05f;
}

void InputComponent::Finalize()
{
}
