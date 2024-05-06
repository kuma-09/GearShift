#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"


class Enemy
{
public:

	void Initialize();

	void Update();

	void Render();

	void Finalize();

	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitive;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	DirectX::VertexPositionColor m_vertices[4];

	DirectX::SimpleMath::Vector3 m_position;
};

