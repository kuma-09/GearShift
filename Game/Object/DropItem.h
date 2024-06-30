#pragma once
#include "Game/GameObject.h"
#include "Game/Menu/Menu.h"


class DropItem :public GameObject
{
public:
	DropItem(IScene* scene);
	~DropItem();
	
	void Update(float elapsedTime);
	void Render();

	void SetHit(bool isHit) { m_isHit = isHit; }

private:
	std::unique_ptr<Menu> m_menu;
	bool m_isHit;
};
