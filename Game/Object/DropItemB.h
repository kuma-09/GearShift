#pragma once
#include "Game/GameObject.h"
#include "Game/UI/UI.h"
#include "Game/Object/Bullet/Bullet.h"

class DropItemB :public GameObject
{
private:


public:
	DropItemB(IScene* scene, std::vector<std::unique_ptr<Bullet>> wepons);
	~DropItemB();
	
	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	std::vector<std::unique_ptr<Bullet>> GetPart() { return std::move(m_bullet); }
	void SetHit(bool isHit) { m_isHit = isHit; }

private:
	std::unique_ptr<UI> m_menu;
	std::vector<std::unique_ptr<Bullet>> m_bullet;
	const float m_dropItemModelSize = 0.1f;
	bool m_isHit;
};
