#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "map"

class HitEffect
{
public:



	void Set(DirectX::SimpleMath::Vector3 pos);

	void Update(float elapsedTime);

	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void Finalize();

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	DirectX::VertexPositionTexture m_vertices[4];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	std::map<DirectX::SimpleMath::Vector3, float> m_posLife;
	const float MAX_LIFETIME = 9;

	int m_u;
	int m_v;

	

};