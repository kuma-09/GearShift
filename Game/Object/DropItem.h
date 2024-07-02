#pragma once
#include "Game/GameObject.h"
#include "Game/Menu/Menu.h"

class IPart;

class DropItem :public GameObject
{
public:
	DropItem(IScene* scene, std::shared_ptr<IPart> part);
	~DropItem();
	
	void Update(float elapsedTime);
	void Render();

	std::shared_ptr<IPart> GetPart() { return m_part; }
	void SetHit(bool isHit) { m_isHit = isHit; }

private:
	std::unique_ptr<Menu> m_menu;
	std::shared_ptr<IPart> m_part;
	bool m_isHit;
};
