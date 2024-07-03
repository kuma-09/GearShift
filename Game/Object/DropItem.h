#pragma once
#include "Game/GameObject.h"
#include "Game/Menu/Menu.h"

class IPart;

class DropItem :public GameObject
{
public:
	DropItem(IScene* scene, IPart* part);
	~DropItem();
	
	void Update(float elapsedTime);
	void Render();

	IPart* GetPart() { return m_part; }
	void SetHit(bool isHit) { m_isHit = isHit; }

private:
	std::unique_ptr<Menu> m_menu;
	IPart* m_part;
	bool m_isHit;
};
