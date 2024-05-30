#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Player/Particle.h"


class Player;
class Enemy;
class Camera;
class Wall;

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

	Player* m_player;

	Camera* m_camera;

	std::vector<Enemy*> m_enemy;
	int m_enemyNum;

	std::vector<Wall*> m_wall;
	std::unique_ptr<Particle> m_particle;
	
	DirectX::VertexPositionColorTexture	m_vertices[4];				// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	
};

