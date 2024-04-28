#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"


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
	DirectX::BasicEffect* m_basicEffect;
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* m_primitiveBatch;

	std::unique_ptr<DirectX::Model> m_model;

	float m_angle;
	DirectX::SimpleMath::Quaternion m_quaternion;

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_center;

	DirectX::VertexPositionColorTexture	m_vertices[4];				// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture; // テクスチャ
	
};

