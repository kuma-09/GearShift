#pragma once
#include "Game/GameObject.h"
#include "Game/UI/UI.h"
#include "Game/Object/Bullet/Bullet.h"

class DropItemB :public GameObject
{
private:


public:
	DropItemB(IScene* scene);
	~DropItemB();
	
	void Initialize();
	void Update(float elapsedTime);
	void CreateShadow();
	void Render();
	void Finalize();

	void SetHit(bool isHit) { m_isHit = isHit; }

	void Collision(Collider* collider);

private:
	const float m_dropItemModelSize = 0.1f;
	bool m_isHit;
};
