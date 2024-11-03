#include "pch.h"	
#include "HitEffect.h"
#include "Framework/Graphics.h"

void HitEffect::Initialize()
{
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(m_graphics->GetDeviceResources()->GetD3DDeviceContext());
	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
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


	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,									// デバイスコンテキスト
			L"Resources/Textures/smoke_white_big.png",	// 画像ファイルのパス
			nullptr,								// 内部的なテクスチャ
			m_texture.ReleaseAndGetAddressOf()		// シェーダリソースビュー(表示用)
		)
	);
}

void HitEffect::Set(DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;



	for (int i = 0; i < 3; i++)
	{
		m_posLife.insert({ pos,0.0f });
	}


	m_u = 0;
	m_v = 0;
}

void HitEffect::Update(float elapsedTime)
{

	for (auto it = m_posLife.begin(); it != m_posLife.end();)
	{
		it->second += elapsedTime;
		if (it->second >= MAX_LIFETIME)
		{
			it = m_posLife.erase(it);
		}
		else
		{
			it++;
		}
	}

}

void HitEffect::Render()
{
	using namespace DirectX::SimpleMath;

	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto proj = Graphics::GetInstance()->GetProjectionMatrix();

	// 各種パラメータを更新する
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);// ブレンドステート
	ID3D11SamplerState* sampler = states->PointWrap();	// or PointWrap()
	context->PSSetSamplers(0, 1, &sampler);
	context->OMSetDepthStencilState(states->DepthRead(), 0);		// 深度バッファ/ステンシルバッファ
	context->RSSetState(states->CullCounterClockwise());						// カリング
	context->IASetInputLayout(m_inputLayout.Get());					// 入力レイアウト




	for (auto& posLife : m_posLife)
	{
		// 各種変換
		Matrix world = Matrix::CreateScale(1.0f * (0.1f + posLife.second));
		world *= Matrix::CreateTranslation(posLife.first + Vector3{0,posLife.second * 5,0});

		// ワールド行列を更新する
		Matrix billboardMat = view.Invert();
		billboardMat._41 = 0;
		billboardMat._42 = 0;
		billboardMat._43 = 0;
		billboardMat *= world;


		// エフェクトを変更する→座標系を設定する
		m_basicEffect->SetWorld(billboardMat);				// ワールド行列
		m_basicEffect->SetView(view);				// ビュー行列
		m_basicEffect->SetProjection(proj);	// 射影行列
		m_basicEffect->SetTexture(m_texture.Get());	// テクスチャ
		m_basicEffect->SetColorAndAlpha({1, 0.25f,0, 1 - posLife.second});
		m_basicEffect->Apply(context);				// ベーシックエフェクトを更新する

		//int n = int(posLife.second);
		//int y = n / 3;
		//int x = n % 3;

		//// UV座標指定用の配列
		//constexpr float u[4]{ 0.0f, 0.34f, 0.67f, 1.0f };
		//constexpr float v[4]{ 0.0f, 0.34f, 0.67f, 1.0f };

		//m_vertices[0] = { Vector3(-3.0f , 3.0f,0), Vector2(u[x],     v[y]) };	//左上
		//m_vertices[1] = { Vector3( 3.0f , 3.0f,0),  Vector2(u[x + 1], v[y]) };	//右上
		//m_vertices[2] = { Vector3(-3.0f ,-3.0f,0), Vector2(u[x],     v[y + 1]) };	//左下
		//m_vertices[3] = { Vector3( 3.0f ,-3.0f,0),  Vector2(u[x + 1], v[y + 1]) };	//右下

		m_vertices[0] = { Vector3(-3.0f , 3.0f,0), Vector2( 0, 0) };	//左上
		m_vertices[1] = { Vector3( 3.0f , 3.0f,0), Vector2( 1, 0) };	//右上
		m_vertices[2] = { Vector3(-3.0f ,-3.0f,0), Vector2( 0, 1) };	//左下
		m_vertices[3] = { Vector3( 3.0f ,-3.0f,0), Vector2( 1, 1) };	//右下

		m_batch->Begin();
		m_batch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
		m_batch->End();
	}


}

void HitEffect::Finalize()
{
}
