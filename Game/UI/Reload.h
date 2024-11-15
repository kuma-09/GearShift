#pragma once
#include "UI.h"


class Reload
{
public:
	Reload();
	~Reload();

	void Initialize();
	void Update(float elapsedTime);
	void Render();

private:
	float m_nowTime;
	std::unique_ptr<UI> m_ui;
	DirectX::XMVECTORF32 m_color;


};