#include "pch.h"
#include "Trail.h"
#include "Framework/BinaryFile.h"
#include "Game/Manager/RenderManager.h"

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Trail::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// コンストラクタ
Trail::Trail()
{
	// レンダーマネージャーにトレイルを追加
	RenderManager::Add(this);

	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// プリミティブバッチを作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);
	m_bufferCount = 0;

	// トレイル用のシェーダーを読み込む
	BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/TrailVS.cso");
	BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/TrailPS.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		vs.GetData(), vs.GetSize(),
		m_inputLayout.GetAddressOf()
	);

	// 頂点シェーダーの作成
	DX::ThrowIfFailed(
		device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);
	// ピクセルシェーダーの作成
	DX::ThrowIfFailed(
		device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);

	// 定数バッファの作成
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));

}

// デストラクタ
Trail::~Trail()
{
	RenderManager::Remove(this);
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="path">トレイルで使うテクスチャ画像のパス</param>
/// <param name="bufferSize">トレイルの頂点数</param>
/// <param name="color">トレイルのカラー</param>
void Trail::Initialize(const wchar_t* path, int bufferSize, DirectX::XMVECTORF32 color)
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();

	m_color = color;
	m_maxBufferSize = bufferSize;
	m_buffer.resize(bufferSize);

	// テクスチャをロードする
	DirectX::CreateWICTextureFromFile(
		device,
		path,
		nullptr,
		m_texture.GetAddressOf()
	);
}

// 更新処理
void Trail::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

// 描画処理
void Trail::Render()
{
	using namespace DirectX::SimpleMath;
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto state = Graphics::GetInstance()->GetCommonStates();
	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto proj = Graphics::GetInstance()->GetProjectionMatrix();
	
	//	半透明描画指定
	ID3D11BlendState* blendstate = state->NonPremultiplied();
	//	透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 定数バッファを更新
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// GPUが定数バッファに対してアクセスを行わないようにする
	context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstantBuffer cb = {};
	cb.world = Matrix::Identity.Transpose();
	cb.view = view.Transpose();
	cb.proj = proj.Transpose();
	cb.color = m_color;
	*static_cast<ConstantBuffer*>(mappedResource.pData) = cb;
	// GPUが定数バッファに対してのアクセスを許可する
	context->Unmap(m_constantBuffer.Get(), 0);
	// ピクセルシェーダ使用する定数バッファを設定
	ID3D11Buffer* cbuffer[] = { m_constantBuffer.Get() };
	context->PSSetConstantBuffers(1, 1, cbuffer);
	context->VSSetConstantBuffers(0, 1, cbuffer);

	ID3D11SamplerState* sampler[1] = { state->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->RSSetState(state->CullNone());
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->OMSetDepthStencilState(state->DepthRead(), 0);
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->IASetInputLayout(m_inputLayout.Get());

	// トレイルを描画
	m_batch->Begin();
	if (m_bufferCount >= 2)
	{
		int count = std::min(m_bufferCount, m_maxBufferSize);
		float uv = 1.0f / float(count);
		for (int i = 1; i < count; i++)
		{
			DirectX::VertexPositionTexture vertex[4] =
			{
				{Vector3(m_buffer[i - 1].head)  ,Vector2(0.0f + uv * (i - 1), 0)},
				{Vector3(m_buffer[i    ].head)  ,Vector2(0.0f + uv * i      , 0)},
				{Vector3(m_buffer[i - 1].tail)  ,Vector2(0.0f + uv * (i - 1), 1)},
				{Vector3(m_buffer[i    ].tail)  ,Vector2(0.0f + uv * i      , 1)},
			};

			m_batch->DrawQuad(vertex[0], vertex[1], vertex[3], vertex[2]);
		}
	}
	m_batch->End();
}

// トレイルを生成する座標を保存
void Trail::SetPos(DirectX::XMFLOAT3 head, DirectX::XMFLOAT3 tail)
{
	PosBuffer tmp;
	tmp.head = head;
	tmp.tail = tail;
	if (m_bufferCount >= m_maxBufferSize)
	{
		for (int i = 0; i < m_maxBufferSize - 1; i++)
		{
			m_buffer[i] = m_buffer[i + 1];
			m_buffer.back() = tmp;
		}
	}
	else
	{
		m_buffer.at(m_bufferCount) = tmp;
	}
	m_bufferCount++;
}

void Trail::ClearBuffer()
{
	m_buffer.clear();
	m_buffer.resize(m_maxBufferSize);
	m_bufferCount = 0;
}
