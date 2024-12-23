/*
	@file	InputManager.cpp
	@brief	�}�E�X�ƃL�[�{�[�h�̓��͂𐿂������N���X
*/
#include "pch.h"
#include "InputManager.h"

std::unique_ptr<InputManager> InputManager::s_inputManager = nullptr;

InputManager* const InputManager::GetInstance()
{
	if (s_inputManager == nullptr)
	{
		s_inputManager.reset(new InputManager());
	}
	return s_inputManager.get();
}

InputManager::InputManager()
	:
	m_mouseState{},
	m_keyboardState{},
	m_gamepadState{}
{

}

//---------------------------------------------------------
// ������
//---------------------------------------------------------
void InputManager::Initialize(const HWND& window)
{
	// �}�E�X���g�p�ł����Ԃɂ���
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(window);
	m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	m_mouseTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	// �L�[�{�[�h���g�p�ł����Ԃɂ���
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_keyboardTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// �Q�[���p�b�h���g�p�ł����Ԃɂ���
	m_gamepad = std::make_unique<DirectX::GamePad>();
	m_gamepadTracker = std::make_unique<DirectX::GamePad::ButtonStateTracker>();
}

//---------------------------------------------------------
// �}�l�[�W���̎����\�[�X���X�V����
//---------------------------------------------------------
void InputManager::Update()
{
	using namespace DirectX;

	// �}�E�X�̏����X�V����
	m_mouseState = m_mouse->GetState();
	m_mouseTracker->Update(m_mouseState);
	

	// �L�[�{�[�h�̏����X�V����
	m_keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(m_keyboardState);

	// �Q�[���p�b�h�̏����X�V����
	m_gamepadState = m_gamepad->GetState(0, DirectX::GamePad::DEAD_ZONE_CIRCULAR);
	m_gamepadTracker->Update(m_gamepadState);
}
