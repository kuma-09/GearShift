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

class Camera;

class PlayScene final: public IScene
{
public:
	PlayScene();

	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;

private:
	std::vector<BoxCollider*> GetHitBoxCollider(BoxCollider::TypeID target, BoxCollider::TypeID object);
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

	
};

