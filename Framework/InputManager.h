#pragma once

/// <summary>
/// 入力情報を更新/取得するクラス
/// シングルトン
/// </summary>
class InputManager
{
public:

	// マウスステートを取得する
	const DirectX::Mouse::State& GetMouseState() const{ return m_mouseState; }
	// マウスのボタンステートトラッカーを取得する
	const std::unique_ptr<DirectX::Mouse::ButtonStateTracker>& GetMouseTracker() const { return m_mouseTracker; }
	// キーボードステートを取得する
	const DirectX::Keyboard::State& GetKeyboardState() const { return m_keyboardState; }
	// キーボードのステートトラッカーを取得する
	const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& GetKeyboardTracker() const { return m_keyboardTracker; }
	// ゲームパッドステートを取得する
	const DirectX::GamePad::State& GetGamePadState() const { return m_gamepadState; }
	// ゲームパッドのステートトラッカーを取得する
	const std::unique_ptr<DirectX::GamePad::ButtonStateTracker>& GetGamePadTracker() const { return m_gamepadTracker; }

private:
	// コンストラクタ
	InputManager();
	// インスタンスをコピーすることを禁止する
	void operator=(const InputManager&) = delete;
	// インスタンスをムーブすることを禁止する
	InputManager& operator= (InputManager&&) = delete;
	// コピーコンストラクタは禁止する
	InputManager(const InputManager&) = delete;
	// ムーブコンストラクタは禁止する
	InputManager(InputManager&&) = delete;

public:
	// インスタンスを取得
	static InputManager* const GetInstance();

	// 初期化処理
	void Initialize(const HWND& window);
	// 更新処理
	void Update();

private:
	
	// インスタンス
	static std::unique_ptr<InputManager> s_inputManager;

	// マウス
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouseTracker;
	DirectX::Mouse::State m_mouseState;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;
	DirectX::Keyboard::State m_keyboardState;

	// ゲームパッド
	std::unique_ptr<DirectX::GamePad> m_gamepad;
	std::unique_ptr<DirectX::GamePad::ButtonStateTracker> m_gamepadTracker;
	DirectX::GamePad::State m_gamepadState;

};