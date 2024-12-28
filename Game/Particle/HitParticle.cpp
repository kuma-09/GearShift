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
	m_basicEffect{},
	m_primitiveBatch{},
	m_inputLayout{},
	m_projection{},
	ALPHA{},
	RG{},
	m_vertices{}
{
	
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
void HitParticle::Initialize(Vector3 pos, Vector3 vel)
{

	

	auto device =  Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();



	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(false);	// ライティング(OFF)
	m_basicEffect->SetVertexColorEnabled(false);// 頂点カラー(OFF)
	m_basicEffect->SetTextureEnabled(true);		// テクスチャ(ON)

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	

	m_vertices[0] = {Vector3( 0.0f, 0.0f, 0.0f) ,  Vector2(0.0f,0.0f)};
	m_vertices[1] = {Vector3( 0.25f, 0.0f, 0.0f) , Vector2(1.0f,0.0f)};
	m_vertices[2] = {Vector3( 0.0f, 0.25f, 0.0f) , Vector2(0.0f,1.0f)};
	m_vertices[3] = {Vector3( 0.25f, 0.25f, 0.0f) ,Vector2(1.0f,1.0f)};
	m_pos = pos;
	m_vel = vel;
	// 色とアルファ値を初期化
	ALPHA = 1.0f;
	RG = 0.0f;

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

	// 各種パラメータを更新する
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);// ブレンドステート
	ID3D11SamplerState* sampler = states->PointWrap();	// or PointWrap()
	context->PSSetSamplers(0, 1, &sampler);
	context->OMSetDepthStencilState(states->DepthRead(), 0);		// 深度バッファ/ステンシルバッファ
	context->RSSetState(states->CullClockwise());						// カリング
	context->IASetInputLayout(m_inputLayout.Get());					// 入力レイアウト


	// ワールド行列を更新する
	Matrix billboardMat = view.Invert();
	billboardMat._41 = 0;
	billboardMat._42 = 0;
	billboardMat._43 = 0;

	// 各種変換
	Matrix world = Matrix::CreateScale(1.0f);
	world *= Matrix::CreateTranslation(m_pos);
	billboardMat *= world;


	// エフェクトを変更する→座標系を設定する
	m_basicEffect->SetWorld(billboardMat);				// ワールド行列
	m_basicEffect->SetView(view);				// ビュー行列
	m_basicEffect->SetProjection(projection);	// 射影行列
	m_basicEffect->SetTexture(m_texture.Get());	// テクスチャ
	m_basicEffect->SetColorAndAlpha(Vector4(0.5f, 0.7f, 1.f, ALPHA));



	m_basicEffect->Apply(context);				// ベーシックエフェクトを更新する


	// プリミティブバッチで描画する
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
	m_primitiveBatch->End();

}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void HitParticle::Finalize()
{
	// do nothing.
}
