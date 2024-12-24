#pragma once

#include "BurnerVertical.h"
#include "BurnerHorizon.h"

class Burner
{
public:
	Burner();
	~Burner();
	void Initialize();
	void Update(float elapsedTime ,DirectX::SimpleMath::Vector3 pos,DirectX::SimpleMath::Quaternion rotate);

	GameObject* GetOwner() { return m_owner; }
private:
	GameObject* m_owner;
	std::unique_ptr<BurnerVertical> m_vertical;
	std::unique_ptr<BurnerHorizon>  m_horizon;
};