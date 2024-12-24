#pragma once

#include "BurnerVertical.h"
#include "BurnerHorizon.h"

class Burner
{
public:
	Burner();
	~Burner();
	void Initialize();
	void Update(DirectX::SimpleMath::Vector3 pos);
private:
	std::unique_ptr<BurnerVertical> m_vertical;
};