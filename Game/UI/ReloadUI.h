#pragma once
#include "UI.h"


class ReloadUI
{
public:
	ReloadUI();
	~ReloadUI();

	void Initialize();
	void Update(float elapsedTime);
	void Render();

private:
	float m_nowTime;
	std::unique_ptr<UI> m_ui;
	DirectX::XMVECTORF32 m_color;


};