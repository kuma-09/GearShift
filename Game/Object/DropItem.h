#pragma once
#include "Game/GameObject.h"
#include "Game/Menu/Menu.h"

class Part;

class DropItem :public GameObject
{
private:


public:
	DropItem(IScene* scene, std::unique_ptr<Part> part);
	~DropItem();
	
	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	std::unique_ptr<Part> GetPart() { return std::move(m_part); }
	void SetHit(bool isHit) { m_isHit = isHit; }

private:
	std::unique_ptr<Menu> m_menu;
	std::unique_ptr<Part> m_part;
	bool m_isHit;
};
