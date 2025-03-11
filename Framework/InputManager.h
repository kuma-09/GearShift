#pragma once

/// <summary>
/// ���͏����X�V/�擾����N���X
/// �V���O���g��
/// </summary>
class InputManager
{
public:

	// �}�E�X�X�e�[�g���擾����
	const DirectX::Mouse::State& GetMouseState() const{ return m_mouseState; }
	// �}�E�X�̃{�^���X�e�[�g�g���b�J�[���擾����
	const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>& GetMouseTracker() const { return m_mouseTracker; }
	// �L�[�{�[�h�X�e�[�g���擾����
	const DirectX::Keyboard::State& GetKeyboardState() const { return m_keyboardState; }
	// �L�[�{�[�h�̃X�e�[�g�g���b�J�[���擾����
	const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& GetKeyboardTracker() const { return m_keyboardTracker; }
	// �Q�[���p�b�h�X�e�[�g���擾����
	const DirectX::GamePad::State& GetGamePadState() const { return m_gamepadState; }
	// �Q�[���p�b�h�̃X�e�[�g�g���b�J�[���擾����
	const std::unique_ptr<DirectX::GamePad::ButtonStateTracker>& GetGamePadTracker() const { return m_gamepadTracker; }

private:
	// �R���X�g���N�^
	InputManager();
	// �C���X�^���X���R�s�[���邱�Ƃ��֎~����
	void operator=(const InputManager&) = delete;
	// �C���X�^���X�����[�u���邱�Ƃ��֎~����
	InputManager& operator= (InputManager&&) = delete;
	// �R�s�[�R���X�g���N�^�͋֎~����
	InputManager(const InputManager&) = delete;
	// ���[�u�R���X�g���N�^�͋֎~����
	InputManager(InputManager&&) = delete;

public:
	// �C���X�^���X���擾
	static InputManager* const GetInstance();

	// ����������
	void Initialize(const HWND& window);
	// �X�V����
	void Update();

private:
	
	// �C���X�^���X
	static std::unique_ptr<InputManager> s_inputManager;

	// �}�E�X
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouseTracker;
	DirectX::Mouse::State m_mouseState;

	// �L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;
	DirectX::Keyboard::State m_keyboardState;

	// �Q�[���p�b�h
	std::unique_ptr<DirectX::GamePad> m_gamepad;
	std::unique_ptr<DirectX::GamePad::ButtonStateTracker> m_gamepadTracker;
	DirectX::GamePad::State m_gamepadState;

};