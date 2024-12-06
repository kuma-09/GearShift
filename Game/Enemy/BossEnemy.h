#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Enemy.h"
#include "Game/Parts/Part.h"

#include "Game/Object/Bullet/LaserBullet.h"
#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Bullet/EnemyBullet.h"

#include "Game/Enemy/BossState/BossMissileState.h"
#include "Game/Enemy/BossState/BossTackleState.h"

class State;

class BossEnemy : public Enemy
{
public:

	BossEnemy(IScene* scene, GameObject* target);
	~BossEnemy();

	void Initialize();
	void Update(float elapsedTime);
	void CreateShader();
	void Render();
	void Finalize();

	void Shot() {};
	void ShotMissile();
	void ShotGatling();

	void ReloadGatling();

	void ChangeState(State* state);

	State* GetMissileState() { return m_missileState.get(); }
	State* GetTackleState() { return m_tackleState.get(); }

	void Collision(Collider* collider);

	// パーツをセット
	void SetPart(const Part::TypeID& partType, std::unique_ptr<Part> part)
	{
		m_pPart[partType] = std::move(part);
		m_pPart[partType]->SetOwner(this);
		m_pPart[partType]->Initialize(10,GetScene());
	}

	// パーツを取得
	Part* GetPart(const Part::TypeID& partType)
	{
		if (m_pPart.find(partType) != m_pPart.end())
		{
			return m_pPart[partType].get();
		}
		return nullptr;
	}

private:

	// パーツをまとめて更新
	void UpdateParts(float elapsedTime) {
		for (auto& pair : m_pPart)
		{
			pair.second->Update(elapsedTime);
		}
	}

	// パーツをまとめて描画
	void RenderParts() {
		for (auto& pair : m_pPart)
		{
			pair.second->Render();
		}
	}

	void CreateShadows()
	{
		for (auto& pair : m_pPart)
		{
			pair.second->CreateShadow();
		}
	}

	// パーツ配列
	std::unordered_map<Part::TypeID, std::unique_ptr<Part>> m_pPart;

	const int MAX_LASER_BULLET = 10;
	const int MAX_FIXED_BULLET = 10;
	const int MAX_HOMING_BULLET = 10;

	std::vector<std::unique_ptr<LaserBullet>>      m_laserBullet;
	std::vector<std::unique_ptr<EnemyBullet>> m_fixedBullets;
	std::vector<std::unique_ptr<HomingBullet>>     m_homingBullets;

	State* m_state;
	std::unique_ptr<BossMissileState> m_missileState;
	std::unique_ptr<BossTackleState> m_tackleState;
};