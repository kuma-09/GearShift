#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Particle/Emitter.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Game/Object/Wall.h"
#include "Game/Object/DropItem.h"
#include "Game/Object/SkyDome.h"

#include "Framework/DebugString.h"

class Camera;

class HitParticle;

class PlayScene final: public IScene
{
public:
	PlayScene();
	~PlayScene();

	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;

	// コンポーネントを追加
	void AddCollider(BoxCollider* collider)
	{
		m_pBoxCollider.push_back(collider);
	}

	void RemoveCollider(BoxCollider* collider)
	{
		int it = 0;
		for (auto boxColliders : m_pBoxCollider)
		{
			if (boxColliders == collider)
			{
				break;
			}
			it++;
		}

		m_pBoxCollider.erase(m_pBoxCollider.begin() + it);
	}

	std::vector<BoxCollider*> GetColliders() { return m_pBoxCollider; }
	void ClearColliders() 
	{
		m_pBoxCollider.clear();
	}

	void CreateHitParticle(DirectX::SimpleMath::Matrix world);

private:
	void NextTarget();
private:


	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;


	std::unique_ptr<DirectX::BoundingBox> m_enemyBox;

	std::unique_ptr<Player> m_player;

	std::vector<std::unique_ptr<Enemy>> m_enemy;
	int m_enemyNum;

	std::vector<std::unique_ptr<Wall>> m_wall;
	std::vector<std::unique_ptr<DropItem>> m_dropItem;
	std::unique_ptr<SkyDome> m_skyDome;

	std::vector<BoxCollider*> m_pBoxCollider;

	std::unique_ptr<DebugString> m_debugString;

	std::vector<std::unique_ptr<HitParticle>> m_hitParticle;

	float m_totalTime;
	float m_timeLimit;
	
};

