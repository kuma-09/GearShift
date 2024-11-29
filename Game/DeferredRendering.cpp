#include "pch.h"
#include "DeferredRendering.h"
#include "Framework/Graphics.h"
#include "Framework/BinaryFile.h"

const std::vector<D3D11_INPUT_ELEMENT_DESC> DeferredRendering::INPUT_LAYOUT_G =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR" ,   0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const std::vector<D3D11_INPUT_ELEMENT_DESC> DeferredRendering::INPUT_LAYOUT_L =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// 静的メンバ変数の初期化
std::unique_ptr<DX::RenderTexture> DeferredRendering::m_albedoRT;
std::unique_ptr<DX::RenderTexture> DeferredRendering::m_normalRT;
std::unique_ptr<DX::RenderTexture> DeferredRendering::m_depthRT;
std::unique_ptr<DX::RenderTexture> DeferredRendering::m_deferredRT;
Microsoft::WRL::ComPtr<ID3D11VertexShader> DeferredRendering::m_vertexShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader> DeferredRendering::m_pixelShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader> DeferredRendering::m_pixelShader_tex;
Microsoft::WRL::ComPtr<ID3D11VertexShader> DeferredRendering::m_vertexShader_light;
Microsoft::WRL::ComPtr<ID3D11PixelShader> DeferredRendering::m_pixelShader_light;
Microsoft::WRL::ComPtr<ID3D11VertexShader> DeferredRendering::m_vertexShader_combient;
Microsoft::WRL::ComPtr<ID3D11PixelShader> DeferredRendering::m_pixelShader_combient;
Microsoft::WRL::ComPtr<ID3D11Buffer> DeferredRendering::m_constantBuffer;

Microsoft::WRL::ComPtr<ID3D11InputLayout> DeferredRendering::m_inputLayoutGBuffer;
Microsoft::WRL::ComPtr<ID3D11InputLayout> DeferredRendering::m_inputLayoutLight;
// プリミティブバッチ
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> DeferredRendering::m_batch;
// 頂点情報
DirectX::VertexPositionTexture DeferredRendering::m_vertex[4];

void DeferredRendering::Initialize()
{
	using namespace DirectX::SimpleMath;

	m_albedoRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);
	m_normalRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R10G10B10A2_UNORM);
	m_depthRT  = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	m_deferredRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);

	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	m_albedoRT->SetDevice(device);
	m_normalRT->SetDevice(device);
	m_depthRT->SetDevice(device);
	m_deferredRT->SetDevice(device);

	m_albedoRT->SetWindow(rect);
	m_normalRT->SetWindow(rect);
	m_depthRT->SetWindow(rect);
	m_deferredRT->SetWindow(rect);

	BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/GBufferVS.cso");
	BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/GBufferPS.cso");
	BinaryFile ps_tex = BinaryFile::LoadFile(L"Resources/Shaders/GBufferPS_Tex.cso");
	BinaryFile vs_deferred = BinaryFile::LoadFile(L"Resources/Shaders/DeferredShadingVS.cso");
	BinaryFile ps_deferred = BinaryFile::LoadFile(L"Resources/Shaders/DeferredShadingPS.cso");
	BinaryFile vs_combient = BinaryFile::LoadFile(L"Resources/Shaders/CombientVS.cso");
	BinaryFile ps_combient = BinaryFile::LoadFile(L"Resources/Shaders/CombientPS.cso");

	device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_tex.GetData(), ps_tex.GetSize(), nullptr, m_pixelShader_tex.ReleaseAndGetAddressOf());
	device->CreateVertexShader(vs_deferred.GetData(), vs_deferred.GetSize(), nullptr, m_vertexShader_light.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_deferred.GetData(), ps_deferred.GetSize(), nullptr, m_pixelShader_light.ReleaseAndGetAddressOf());
	device->CreateVertexShader(vs_combient.GetData(), vs_combient.GetSize(), nullptr, m_vertexShader_combient.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_combient.GetData(), ps_combient.GetSize(), nullptr, m_pixelShader_combient.ReleaseAndGetAddressOf());


	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT_G[0],
		static_cast<UINT>(INPUT_LAYOUT_G.size()),
		vs.GetData(), vs.GetSize(),
		m_inputLayoutGBuffer.GetAddressOf(
	));

	device->CreateInputLayout(&INPUT_LAYOUT_L[0],
		static_cast<UINT>(INPUT_LAYOUT_L.size()),
		vs_deferred.GetData(), vs_deferred.GetSize(),
		m_inputLayoutLight.GetAddressOf(
		));

	// 定数バッファ用のバッファオブジェクトを作成する
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstBuffer));	// 16の倍数を指定する
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(
		device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf())
	);

	// プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	m_vertex[0] = { Vector3(-1.0f , 1.0f,0) , Vector2(0, 0) };	//左上
	m_vertex[1] = { Vector3( 1.0f , 1.0f,0) , Vector2(1, 0) };	//右上
	m_vertex[2] = { Vector3(-1.0f ,-1.0f,0) , Vector2(0, 1) };	//左下
	m_vertex[3] = { Vector3( 1.0f ,-1.0f,0) , Vector2(1, 1) };	//右下
}

void DeferredRendering::BeginGBuffer()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto depthStencil = Graphics::GetInstance()->GetDeviceResources()->GetDepthStencilView();
	auto albedoRTV = m_albedoRT->GetRenderTargetView();
	auto normalRTV = m_normalRT->GetRenderTargetView();
	auto depthRTV = m_depthRT->GetRenderTargetView();

	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto projection = Graphics::GetInstance()->GetProjectionMatrix();

	ID3D11RenderTargetView* renderTargets[3] = { albedoRTV, normalRTV,depthRTV };
	// -------------------------------------------------------------------------- //
	// レンダーターゲットを変更
	// -------------------------------------------------------------------------- //
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	for (auto& rtv : renderTargets)
	{
		context->ClearRenderTargetView(rtv, DirectX::Colors::Black);
	}
	context->OMSetRenderTargets(3, renderTargets, depthStencil);

	// 定数バッファをマップする
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer* cb = static_cast<ConstBuffer*>(mappedResource.pData);
	auto viewproj = view * projection;
	cb->matView = view.Transpose();
	cb->matProj = projection.Transpose();
	cb->inverseViewProj = DirectX::SimpleMath::Matrix::Identity.Invert();
	// マップを解除する
	context->Unmap(m_constantBuffer.Get(), 0);

	// 定数バッファの設定
	ID3D11Buffer* cbuf[] = { m_constantBuffer.Get() };
	context->PSSetConstantBuffers(1, 1, cbuf);
}

void DeferredRendering::DrawGBuffer(bool texture)
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto state = Graphics::GetInstance()->GetCommonStates();
	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto projection = Graphics::GetInstance()->GetProjectionMatrix();

	// シェーダを設定する
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	if (texture)
	{
		context->PSSetShader(m_pixelShader_tex.Get(), nullptr, 0);
	}
	else
	{
		context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	}
	context->IASetInputLayout(m_inputLayoutGBuffer.Get());
}

void DeferredRendering::DeferredLighting()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto renderTarget = Graphics::GetInstance()->GetDeviceResources()->GetRenderTargetView();
	auto depthStencil = Graphics::GetInstance()->GetDeviceResources()->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	auto const viewport = Graphics::GetInstance()->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	ID3D11ShaderResourceView* albedo = m_albedoRT->GetShaderResourceView();
	ID3D11ShaderResourceView* normal = m_normalRT->GetShaderResourceView();
	ID3D11ShaderResourceView* depth = m_depthRT->GetShaderResourceView();

	// シェーダを設定する
	context->PSSetShaderResources(1, 1, &albedo);
	context->PSSetShaderResources(2, 1, &normal);
	context->PSSetShaderResources(3, 1, &depth);
	context->VSSetShader(m_vertexShader_light.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader_light.Get(), nullptr, 0);
	context->IASetInputLayout(m_inputLayoutLight.Get());

	m_batch->Begin();
	m_batch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
	m_batch->End();

	// リソースを使用する前にシェーダーリソーススロットを解除
	ID3D11ShaderResourceView* nullSRV[3] = { nullptr,nullptr,nullptr };
	context->PSSetShaderResources(1, 3, nullSRV);
}