#pragma once
#include "Number.h"

class RemainingEnemyUI
{
public:
	RemainingEnemyUI();
	~RemainingEnemyUI();

	void Initialize(DirectX::SimpleMath::Vector2 pos,int num);
	void Update(int num);
	void Render();

private:
	std::unique_ptr<Number> m_nowNumber;
	std::unique_ptr<Number> m_maxNumber;
	std::unique_ptr<Number> m_slash;
};