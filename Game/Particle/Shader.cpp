//--------------------------------------------------------------------------------------
// File: Shader.h
//
// パーティクルクラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Shader.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"
#include "Framework/BinaryFile.h"

using namespace DirectX;

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Shader::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Microsoft::WRL::ComPtr<ID3D11Buffer> Shader::m_CBuffer;

/// <summary>
/// コンストラクタ
/// </summary>
Shader::Shader()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Shader::~Shader()
{
}

/// <summary>
/// Shader作成部分だけ分離した関数
/// </summary>
void Shader::CreateShader()
{
	auto device  = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	//	コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/VS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/PS.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//	シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}


void Shader::RenderStart(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	// コンストバッファを作成
	ConstBuffer cbuff;
	//	ビュー設定
	cbuff.matView = view.Transpose();
	//	プロジェクション設定
	cbuff.matProj = projection.Transpose();
	//	ワールド設定
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//	頂点シェーダもピクセルシェーダも、同じ値を渡す
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);


	ID3D11SamplerState* sampler = states->LinearWrap();
	context->PSSetSamplers(0, 1, &sampler);

	ID3D11BlendState* blendState = states->NonPremultiplied();
	context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullClockwise());

	//	シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());
}

void Shader::RenderEnd()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
