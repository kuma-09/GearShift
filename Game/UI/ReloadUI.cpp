#include "pch.h"
#include "ReloadUI.h"
#include "Framework/BinaryFile.h"

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> ReloadUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR" ,     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ReloadUI::ReloadUI()
{
}

ReloadUI::~ReloadUI()
{
}

void ReloadUI::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	m_graphics = Graphics::GetInstance();
	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();

	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_graphics->GetDeviceResources()->GetD3DDeviceContext());

	m_nowTime = 0;
	m_ui = std::make_unique<UI>(L"Resources/Textures/Reload.png");

	//	コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/FullCircleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/FullCirclePS.cso");

	DirectX::CreateWICTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(), L"Resources/Textures/BulletCircle.png", nullptr, m_texture.ReleaseAndGetAddressOf());

	RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();
	float aspectX = float(rect.right) / float(rect.bottom);

	m_vertex[0] = { Vector3(-0.1f + 0.5f, (-0.1f * aspectX) - 0.9f, 0),Vector4::One, Vector2(0.0f, 0.0f) };		//左上
	m_vertex[1] = { Vector3( 0.1f + 0.5f, (-0.1f * aspectX) - 0.9f, 0),Vector4::One, Vector2(1.0f, 0.0f) };		//右上
	m_vertex[2] = { Vector3(-0.1f + 0.5f, ( 0.1f * aspectX) - 0.9f, 0),Vector4::One, Vector2(0.0f, 1.0f) };		//左下
	m_vertex[3] = { Vector3( 0.1f + 0.5f, ( 0.1f * aspectX) - 0.9f, 0),Vector4::One, Vector2(1.0f, 1.0f) };		//右下

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	頂点シェーダ作成
	device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf());

	//	ピクセルシェーダ作成
	device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf());

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

void ReloadUI::Update(float elapsedTime)
{
	m_nowTime += elapsedTime * 5;
	float n = sinf(m_nowTime) / 2 + 0.5f;
	m_color = { n,n,n,n };
}

void ReloadUI::Render(float reloadTime)
{
	using namespace DirectX::SimpleMath;

	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();

	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.rotate = DirectX::XMConvertToRadians(90) - DirectX::XMConvertToRadians(reloadTime*90);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	半透明描画指定
	ID3D11BlendState* blendstate = state->NonPremultiplied();
	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	//	深度バッファに書き込み参照する
	context->OMSetDepthStencilState(state->DepthNone(), 0);
	//	カリングは左周り
	context->RSSetState(state->CullClockwise());
	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { state->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
	m_batch->End();

	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void ReloadUI::CautionRender()
{
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_ui->Render({ float(windowSize.right) / 2,float(windowSize.bottom) / 2 + 150 }, m_color, DirectX::SimpleMath::Vector2(643, 121) / 2, { 0.25f, 0.25f });
}
