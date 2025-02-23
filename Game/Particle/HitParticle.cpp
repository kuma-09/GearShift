/*
	@file	HitParticle.cpp
	@brief	一般的なシーンクラス
*/
#include "pch.h"
#include "HitParticle.h"
#include <cassert>
#include "Framework/Resources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
HitParticle::HitParticle()
	:
	m_graphics{},
	ALPHA{0},
	RG{},
	m_vertices{}
{

	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,									// デバイスコンテキスト
			L"Resources/Textures/particle.png",	// 画像ファイルのパス
			nullptr,								// 内部的なテクスチャ
			m_texture.ReleaseAndGetAddressOf()		// シェーダリソースビュー(表示用)
		)
	);
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
HitParticle::~HitParticle()
{
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void HitParticle::Initialize(Vector3 pos, Vector3 vel, Vector2 size)
{
	m_vertices[0] = {Vector3( 0.0f, 0.0f, 0.0f)     , Vector4(1,1,1,1), Vector2(0.0f,0.0f)};
	m_vertices[1] = {Vector3( size.x, 0.0f, 0.0f)   , Vector4(1,1,1,1), Vector2(1.0f,0.0f)};
	m_vertices[2] = {Vector3( 0.0f, size.y, 0.0f)   , Vector4(1,1,1,1), Vector2(0.0f,1.0f)};
	m_vertices[3] = {Vector3( size.x, size.y, 0.0f) , Vector4(1,1,1,1), Vector2(1.0f,1.0f)};
	m_pos = pos;
	m_vel = vel;
	// 色とアルファ値を初期化
	ALPHA = 1.0f;
	RG = 0.0f;

}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
bool HitParticle::Update()
{
	m_pos += m_vel;
	ALPHA -= 0.02f;
	if (ALPHA < 0.3f)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void HitParticle::Render(Matrix view, Matrix projection)
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();

	m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());

	// ワールド行列を更新する
	Matrix billboardMat = view.Invert();
	billboardMat._41 = 0;
	billboardMat._42 = 0;
	billboardMat._43 = 0;

	// 各種変換
	Matrix world = Matrix::CreateScale(1.0f);
	world *= Matrix::CreateTranslation(m_pos);
	billboardMat *= world;

	// プリミティブバッチで描画する
	m_graphics->DrawPrimitiveBegin(view, projection, billboardMat, Vector4(0.5f, 0.7f, 1.f, ALPHA));
	m_graphics->GetPrimitiveBatch()->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
	m_graphics->DrawPrimitiveEnd();

}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void HitParticle::Finalize()
{
	// do nothing.
}
