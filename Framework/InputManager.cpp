/*
	@file	InputManager.cpp
	@brief	マウスとキーボードの入力を請け負うクラス
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
// 初期化
//---------------------------------------------------------
void InputManager::Initialize(const HWND& window)
{
	// マウスを使用できる状態にする
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(window);
	m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	m_mouseTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	// キーボードを使用できる状態にする
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_keyboardTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// ゲームパッドを使用できる状態にする
	m_gamepad = std::make_unique<DirectX::GamePad>();
	m_gamepadTracker = std::make_unique<DirectX::GamePad::ButtonStateTracker>();
}

//---------------------------------------------------------
// マネージャの持つリソースを更新する
//---------------------------------------------------------
void InputManager::Update()
{
	using namespace DirectX;

	// マウスの情報を更新する
	m_mouseState = m_mouse->GetState();
	m_mouseTracker->Update(m_mouseState);
	

	// キーボードの情報を更新する
	m_keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(m_keyboardState);

	// ゲームパッドの情報を更新する
	m_gamepadState = m_gamepad->GetState(0, DirectX::GamePad::DEAD_ZONE_CIRCULAR);
	m_gamepadTracker->Update(m_gamepadState);
}
