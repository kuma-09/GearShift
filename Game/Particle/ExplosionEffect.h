#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "map"

class ExplosionEffect
{
public:


	void Initialize();
	void Set(DirectX::SimpleMath::Vector3 pos);

	void Update(float elapsedTime);

	void Render();

	void Finalize();

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	DirectX::VertexPositionColorTexture m_vertices[4];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	std::map<DirectX::SimpleMath::Vector3, float> m_posLife;
	// ï\é¶éûä‘
	const float MAX_LIFETIME = 9;
	// îöïóÇÃêî
	const int   MAX_EFFECT_NUM = 5;


	int m_u;
	int m_v;

	

};