#include "pch.h"
#include "InputComponent.h"

// �R���X�g���N�^
InputComponent::InputComponent()
{
    m_inputManager = InputManager::GetInstance();
}

// �f�X�g���N�^
InputComponent::~InputComponent()
{
}

// ����������
void InputComponent::Initialize()
{
}

// �X�V����
void InputComponent::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;
    UNREFERENCED_PARAMETER(elapsedTime);
    const auto& kb = m_inputManager->GetKeyboardState();
    const auto& gpState = m_inputManager->GetGamePadState();

    // �p�b�h�̓��͏��
    Vector3 input = Vector3::Zero;

    // �p�b�h���q�����Ă��邩
    if (gpState.connected)
    {
        input = Vector3{ gpState.thumbSticks.leftX,0, -gpState.thumbSticks.leftY };
    }
    else
    {
        if (kb.W)
        {
            input.z = -1;
        }
        if (kb.S)
        {
            input.z =  1;
        }
        if (kb.A)
        {
            input.x = -1;
        }
        if (kb.D)
        {
            input.x =  1;
        };
    }

    input.Normalize();
    input *= 0.05f;
    m_velocity = input;
}

