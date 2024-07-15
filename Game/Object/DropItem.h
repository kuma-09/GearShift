#pragma once
#include "Game/GameObject.h"
#include "Game/Menu/Menu.h"

class IPart;

class DropItem :public GameObject
{
private:


public:
	DropItem(IScene* scene, std::unique_ptr<IPart> part);
	~DropItem();
	
	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	std::unique_ptr<IPart> GetPart() { return std::move(m_part); }
	void SetHit(bool isHit) { m_isHit = isHit; }

private:
	std::unique_ptr<Menu> m_menu;
	std::unique_ptr<IPart> m_part;
	bool m_isHit;
};
