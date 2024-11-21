#include "pch.h"
#include "Menu.h"

Menu::Menu()
{
	m_graphics = Graphics::GetInstance();
	m_inputManager = InputManager::GetInstance();
}

Menu::~Menu()
{
}

void Menu::AddUI(const wchar_t* path, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 size)
{
	m_ui.emplace_back(std::make_unique<UI>(path));
	m_pos.emplace_back(pos);
	m_size.emplace_back(size);
}

void Menu::Initialize()
{
	m_menuValue = 0;
	for (int i = 0; i < m_ui.size(); i++)
	{
		m_ui[i]->Initialize();
		m_menuValue++;
	}
	m_activeSize = 0;
	m_activeUI = 0;
}

void Menu::Update()
{
	auto& keyState = m_inputManager->GetKeyboardTracker();

	m_activeSize+=0.1f;
	if (keyState->IsKeyPressed(DirectX::Keyboard::Down))
	{
		m_activeSize = 0;
		m_activeUI++;
		m_activeUI = std::min(m_menuValue - 1, m_activeUI);
	}
	if (keyState->IsKeyPressed(DirectX::Keyboard::Up))
	{
		m_activeSize = 0;
		m_activeUI--;
		m_activeUI = std::max(0, m_activeUI);
	}

}

void Menu::Render(DirectX::SimpleMath::Vector2 pos)
{
	using namespace DirectX;
	for (int i = 0; i < m_menuValue; i++)
	{
		if (m_activeUI == i)
		{
			m_ui[i]->Render(m_pos[i] + pos, Colors::White, SimpleMath::Vector2{700,100} / 2, m_size[i] * (1 + sin(m_activeSize) / 20));
		}
		else
		{
			m_ui[i]->Render(m_pos[i] + pos, Colors::White, SimpleMath::Vector2{ 700,100 } / 2, m_size[i]);
		}
	}
}
