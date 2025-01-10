#include "pch.h"
#include "SceneMask.h"
#include "Framework/BinaryFile.h"

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> SceneMask::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

SceneMask::SceneMask()
	: m_isOpen{},
	  m_isClose{}
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	m_graphics = Graphics::GetInstance();

	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	
	m_batch = std::make_unique<PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/SceneChangeBlack.png",
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);


	m_vertex[0] = { Vector3(-1 ,  1, 0),Vector4::One, Vector2(0.0f, 0.0f) };		//左上
	m_vertex[1] = { Vector3( 1 ,  1, 0),Vector4::One, Vector2(1.0f, 0.0f) };		//右上
	m_vertex[2] = { Vector3(-1 , -1, 0),Vector4::One, Vector2(0.0f, 1.0f) };	//左下
	m_vertex[3] = { Vector3( 1 , -1, 0),Vector4::One, Vector2(1.0f, 1.0f) };		//右下

	BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/SceneMaskVS.cso");
	BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/SceneMaskPS.cso");
	device->CreateVertexShader(vs.GetData(), vs.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps.GetData(), ps.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf());

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		vs.GetData(), vs.GetSize(),
		m_inputLayout.GetAddressOf());

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

}

bool SceneMask::Update(float elapsedTime)
{
	if (m_isOpen)
	{
		// フェードイン
		RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
		m_windowSize = float(rect.right) / float(1280);
		m_time -= elapsedTime * 2;

		if (m_time <= 0)
		{
			m_isOpen = false;
		}
	}
	else if (m_isClose)
	{
		// フェードアウト
		RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
		m_windowSize = float(rect.right) / float(1280);
		m_time += elapsedTime * 2;

		if (m_time >= 1)
		{
			m_isClose = false;
			Open();
			return true;
		}
	}
	return false;
}

void SceneMask::Render()
{
	using namespace DirectX::SimpleMath;


	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();

	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.time = m_time;

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	半透明描画指定
	ID3D11BlendState* blendstate = state->NonPremultiplied();

	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(state->DepthNone(), 0);

	//	カリングは左周り
	context->RSSetState(state->CullNone());
	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { state->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_CBuffer.GetAddressOf());

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	Vector4 alpha = { 1, 1, 1, m_time };
	RECT size = { 0,0,1280,720 };
	m_batch->Begin();
	m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
	m_batch->End();


	//m_spriteBatch->Begin();
	//m_spriteBatch->Draw(m_texture.Get(), Vector2::Zero,&size, alpha, 0.0f, Vector2::Zero,1.0f * m_windowSize);
	//m_spriteBatch->End();
}