#pragma once
#include "Game/GameObject.h"
#include "Game/UI/UI.h"
#include "Game/Parts/Part.h"
#include "Game/Object/Bullet/Bullet.h"

class DropItem :public GameObject
{
private:


public:
	DropItem(IScene* scene);
	DropItem(IScene* scene, std::unique_ptr<Part> part);
	DropItem(IScene* scene, std::unique_ptr<Bullet> bullet);
	~DropItem();
	
	void Initialize();
	void Update(float elapsedTime);
	void CreateShadow();
	void Render();
	void Finalize();

	Part::TypeID GetPartType() { return m_part->GetTypeID(); }
	std::unique_ptr<Part> GetPart() { return std::move(m_part); }
	void SetHit(bool isHit) { m_isHit = isHit; }

	void Collision(Collider* collider);

private:
	std::unique_ptr<Part> m_part;
	const float m_dropItemModelSize = 0.1f;
	bool m_isHit;
};
