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

	void Initialize() override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;

	void CheckHit(DirectX::BoundingBox a, DirectX::BoundingBox b);
private:

	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;


	std::unique_ptr<DirectX::BoundingBox> m_enemyBox;

	std::unique_ptr<Player> m_player;

	Camera* m_camera;

	std::vector<std::unique_ptr<Enemy>> m_enemy;
	int m_enemyNum;

	std::vector<std::unique_ptr<Wall>> m_wall;
	std::vector<std::unique_ptr<DropItem>> m_dropItem;
	std::unique_ptr<SkyDome> m_skyDome;
	
	DirectX::VertexPositionColorTexture	m_vertices[4];				// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	
};

