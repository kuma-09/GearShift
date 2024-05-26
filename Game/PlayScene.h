#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"

class Player;
class Enemy;

class PlayScene final: public IScene
{
public:

	void Initialize() override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;
private:

	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;


	std::unique_ptr<DirectX::BoundingBox> m_enemyBox;

	Player* m_player;

	std::vector<Enemy*> m_enemy;
	int m_enemyNum;
	
	DirectX::VertexPositionColorTexture	m_vertices[4];				// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	
};

