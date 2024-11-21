#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/InputManager.h"
#include "UI.h"

class Menu
{
public:
	Menu();
	~Menu();

	void AddUI(const wchar_t* path, DirectX::SimpleMath::Vector2 pos,DirectX::SimpleMath::Vector2 size = DirectX::SimpleMath::Vector2::One);
	void Initialize();
	void Update();
	void Render(DirectX::SimpleMath::Vector2 pos);

	int GetActiveUI() { return m_activeUI; }

private:

	Graphics* m_graphics;
	InputManager* m_inputManager;
	std::vector<std::unique_ptr<UI>> m_ui;
	std::vector<DirectX::SimpleMath::Vector2> m_pos;
	std::vector<DirectX::SimpleMath::Vector2> m_size;

	float m_activeSize;
	int m_menuValue;
	int m_activeUI;

};