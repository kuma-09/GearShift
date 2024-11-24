#include "pch.h"
#include "Trail.h"
#include "Framework/BinaryFile.h"

/// <summary>
/// インプットレイアウト
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Trail::INPUT_LAYOUT =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Trail::Trail()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);
	m_bufferCount = 0;

	BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/TrailVS.cso");
	BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/TrailPS.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		vs.GetData(), vs.GetSize(),
		m_inputLayout.GetAddressOf()
	);

	DX::ThrowIfFailed(
		device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed(
		device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);


}

void Trail::Initialize(const wchar_t* path, int bufferSize)
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();

	m_maxBufferSize = bufferSize;

	// テクスチャをロードする
	DirectX::CreateWICTextureFromFile(
		device,
		path,
		nullptr,
		m_texture.GetAddressOf()
	);
}

void Trail::Update(float elapsedTime)
{
	
}

void Trail::Render()
{
	using namespace DirectX::SimpleMath;
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto state = Graphics::GetInstance()->GetCommonStates();
	
	ID3D11SamplerState* sampler[1] = { state->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->RSSetState(state->CullNone());
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->OMSetDepthStencilState(state->DepthDefault(), 0);
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->IASetInputLayout(m_inputLayout.Get());
	m_batch->Begin();
	if (m_buffer.size() >= 2)
	{
		for (auto it = m_buffer.begin(); m_buffer.end() - 1 != it; it++)
		{
			DirectX::VertexPositionTexture vertex[4] =
			{
				{Vector3(it->head)      ,Vector2(0.0f,0.0f)},
				{Vector3((it + 1)->head),Vector2(1.0f,0.0f)},
				{Vector3(it->tail)      ,Vector2(0.0f,1.0f)},
				{Vector3((it + 1)->tail),Vector2(1.0f,1.0f)},
			};

			m_batch->DrawQuad(vertex[0], vertex[1], vertex[3], vertex[2]);
		}
	}
	m_batch->End();
}

void Trail::SetPos(DirectX::XMFLOAT3 head, DirectX::XMFLOAT3 tail)
{
	m_bufferCount++;
	PosBuffer tmp;
	tmp.head = head;
	tmp.tail = tail;
	if (m_buffer.size() >= m_maxBufferSize)
	{
		m_buffer.at(m_bufferCount % m_maxBufferSize) = tmp;
	}
	else
	{
		m_buffer.emplace_back(tmp);
	}
}
