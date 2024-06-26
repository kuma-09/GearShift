//--------------------------------------------------------------------------------------
// File: Menu.h
//
// ���j���[�N���X
//
//-------------------------------------------------------------------------------------

#pragma once

#include "Framework/StepTimer.h"
#include "UserInterface.h"
#include "Framework/DebugString.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"

class Menu
{
public:

//	�ϐ�
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;

	std::vector<std::unique_ptr<tito::UserInterface>> m_userInterface;
	std::vector<std::unique_ptr<tito::UserInterface>> m_base;

	const wchar_t* m_baseTexturePath;

	std::unique_ptr<tito::UserInterface> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;
//	�֐�
public:
	Menu();
	~Menu();

	void Initialize(DX::DeviceResources* pDR, int width, int height);
	void Update();
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, tito::ANCHOR anchor);

};