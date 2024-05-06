#include "pch.h"
#include "Gage.h"

void Enemy::Initialize()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	m_deviceResources = m_graphics->GetDeviceResources();

	m_position = Vector3(0, 0, 0);

	m_primitive = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>();

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			m_deviceResources->GetD3DDevice(),
			m_graphics->GetBasicEffect(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	m_vertices[0] = { Vector3(-0.5f, 0.5f,0.0f),Vector4(255,255,255,255) };	// 左上
	m_vertices[1] = { Vector3( 0.5f, 0.5f,0.0f),Vector4(255,255,255,255) };	// 右上
	m_vertices[2] = { Vector3(-0.5f,-0.5f,0.0f),Vector4(255,255,255,255) };	// 左下
	m_vertices[3] = { Vector3( 0.5f,-0.5f,0.0f),Vector4(255,255,255,255) };	// 右下

}

void Enemy::Update()
{

}

void Enemy::Render()
{

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();
	auto basicEffect = m_graphics->GetBasicEffect();

	// ポリゴンを描画するために各種パラメータを設定する
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);	// Tridentロゴは「NonPremultiplied」
	context->OMSetDepthStencilState(states->DepthDefault(), 0);					// 深度情報のreadとwrite
	context->RSSetState(states->CullCounterClockwise());						// ポリゴンの裏面は描画しない
	context->IASetInputLayout(m_inputLayout.Get());								// 適切な入力レイアウトを指定する


	basicEffect->SetView(m_graphics->GetViewMatrix());				// ビュー行列を指定する
	basicEffect->SetProjection(m_graphics->GetProjectionMatrix());	// 射影行列を指定する
	basicEffect->Apply(context);				// ベーシックエフェクトの変更を適用する

	// ビルボードを描画する
	m_primitive->Begin();
	m_primitive->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
	m_primitive->End();
}

void Enemy::Finalize()
{

}