#include "pch.h"
#include "DeferredRendering.h"
#include "Manager/PointLightManager.h"
#include "Game/Shader/ShadowMap.h"
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
std::unique_ptr<DX::RenderTexture> DeferredRendering::s_albedoRT;
std::unique_ptr<DX::RenderTexture> DeferredRendering::s_normalRT;
std::unique_ptr<DX::RenderTexture> DeferredRendering::s_depthRT;
std::unique_ptr<DX::RenderTexture> DeferredRendering::s_finalRT;
Microsoft::WRL::ComPtr<ID3D11VertexShader> DeferredRendering::s_vertexShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader> DeferredRendering::s_pixelShader;
Microsoft::WRL::ComPtr<ID3D11PixelShader> DeferredRendering::s_pixelShader_tex;
Microsoft::WRL::ComPtr<ID3D11VertexShader> DeferredRendering::s_vertexShader_light;
Microsoft::WRL::ComPtr<ID3D11PixelShader> DeferredRendering::s_pixelShader_light;
Microsoft::WRL::ComPtr<ID3D11VertexShader> DeferredRendering::s_vertexShader_combient;
Microsoft::WRL::ComPtr<ID3D11PixelShader> DeferredRendering::s_pixelShader_combient;
Microsoft::WRL::ComPtr<ID3D11Buffer> DeferredRendering::s_constantBuffer;
Microsoft::WRL::ComPtr<ID3D11SamplerState> DeferredRendering::m_shadowMapSampler;
DirectX::SimpleMath::Matrix  DeferredRendering::s_lightViewProj;
DirectX::SimpleMath::Vector3 DeferredRendering::s_lightPosition;

// インプットレイアウト
Microsoft::WRL::ComPtr<ID3D11InputLayout> DeferredRendering::s_inputLayoutGBuffer;
Microsoft::WRL::ComPtr<ID3D11InputLayout> DeferredRendering::m_inputLayoutLight;
// スプライトバッチ
std::unique_ptr<DirectX::SpriteBatch> DeferredRendering::s_spriteBatch;
// プリミティブバッチ
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> DeferredRendering::s_batch;
// 頂点情報
DirectX::VertexPositionTexture DeferredRendering::s_vertex[4];

void DeferredRendering::Initialize()
{
	using namespace DirectX::SimpleMath;

	s_albedoRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);
	s_normalRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R10G10B10A2_UNORM);
	s_depthRT  = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	s_finalRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);

	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	s_albedoRT->SetDevice(device);
	s_normalRT->SetDevice(device);
	s_depthRT->SetDevice(device);
	s_finalRT->SetDevice(device);

	s_albedoRT->SetWindow(rect);
	s_normalRT->SetWindow(rect);
	s_depthRT->SetWindow(rect);
	s_finalRT->SetWindow(rect);
	
	BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/GBufferVS.cso");
	BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/GBufferPS.cso");
	BinaryFile ps_tex = BinaryFile::LoadFile(L"Resources/Shaders/GBufferPS_Tex.cso");
	BinaryFile vs_deferred = BinaryFile::LoadFile(L"Resources/Shaders/DeferredShadingVS.cso");
	BinaryFile ps_deferred = BinaryFile::LoadFile(L"Resources/Shaders/DeferredShadingPS.cso");
	BinaryFile vs_combient = BinaryFile::LoadFile(L"Resources/Shaders/CombientVS.cso");
	BinaryFile ps_combient = BinaryFile::LoadFile(L"Resources/Shaders/CombientPS.cso");

	device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, s_vertexShader.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, s_pixelShader.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_tex.GetData(), ps_tex.GetSize(), nullptr, s_pixelShader_tex.ReleaseAndGetAddressOf());
	device->CreateVertexShader(vs_deferred.GetData(), vs_deferred.GetSize(), nullptr, s_vertexShader_light.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_deferred.GetData(), ps_deferred.GetSize(), nullptr, s_pixelShader_light.ReleaseAndGetAddressOf());
	device->CreateVertexShader(vs_combient.GetData(), vs_combient.GetSize(), nullptr, s_vertexShader_combient.ReleaseAndGetAddressOf());
	device->CreatePixelShader(ps_combient.GetData(), ps_combient.GetSize(), nullptr, s_pixelShader_combient.ReleaseAndGetAddressOf());

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT_G[0],
		static_cast<UINT>(INPUT_LAYOUT_G.size()),
		vs.GetData(), vs.GetSize(),
		s_inputLayoutGBuffer.GetAddressOf(
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
		device->CreateBuffer(&bufferDesc, nullptr, s_constantBuffer.ReleaseAndGetAddressOf())
	);

	// スプライトバッチの生成
	s_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// プリミティブバッチの作成
	s_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	s_vertex[0] = { Vector3(-1.0f , 1.0f,0) , Vector2(0, 0) };	//左上
	s_vertex[1] = { Vector3( 1.0f , 1.0f,0) , Vector2(1, 0) };	//右上
	s_vertex[2] = { Vector3(-1.0f ,-1.0f,0) , Vector2(0, 1) };	//左下
	s_vertex[3] = { Vector3( 1.0f ,-1.0f,0) , Vector2(1, 1) };	//右下

	// サンプラーの作成（シャドウマップ用）
	D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&sampler_desc, m_shadowMapSampler.ReleaseAndGetAddressOf());
}

void DeferredRendering::BeginGBuffer()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states  = Graphics::GetInstance()->GetCommonStates();
	auto depthStencil = Graphics::GetInstance()->GetDeviceResources()->GetDepthStencilView();
	auto albedoRTV = s_albedoRT->GetRenderTargetView();
	auto normalRTV = s_normalRT->GetRenderTargetView();
	auto depthRTV = s_depthRT->GetRenderTargetView();

	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto projection = Graphics::GetInstance()->GetProjectionMatrix();

	ID3D11RenderTargetView* renderTargets[3] = { albedoRTV, normalRTV, depthRTV};
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
	context->Map(s_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer* cb = static_cast<ConstBuffer*>(mappedResource.pData);
	cb->matView = view.Transpose();
	cb->matProj = projection.Transpose();
	cb->inverseViewProj = (view * projection).Invert();
	// マップを解除する
	context->Unmap(s_constantBuffer.Get(), 0);

	// 定数バッファの設定
	ID3D11Buffer* cbuf[] = { s_constantBuffer.Get() };
	context->VSSetConstantBuffers(1, 1, cbuf);
	context->PSSetConstantBuffers(1, 1, cbuf);
}

void DeferredRendering::DrawGBuffer(bool texture)
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto state = Graphics::GetInstance()->GetCommonStates();
	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto projection = Graphics::GetInstance()->GetProjectionMatrix();

	// シェーダを設定する
	context->VSSetShader(s_vertexShader.Get(), nullptr, 0);
	if (texture)
	{
		context->PSSetShader(s_pixelShader_tex.Get(), nullptr, 0);
	}
	else
	{
		context->PSSetShader(s_pixelShader.Get(), nullptr, 0);
	}
	context->IASetInputLayout(s_inputLayoutGBuffer.Get());
}

void DeferredRendering::DeferredLighting()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto renderTarget = s_finalRT->GetRenderTargetView();
	auto depthStencil = Graphics::GetInstance()->GetDeviceResources()->GetDepthStencilView();
	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto projection = Graphics::GetInstance()->GetProjectionMatrix();
	auto states = Graphics::GetInstance()->GetCommonStates();

	//context->ClearRenderTargetView(renderTarget, DirectX::Colors::Black);
	context->OMSetRenderTargets(1, &renderTarget, nullptr);
	context->RSSetState(states->CullNone());						// カリング

	// テクスチャサンプラーの設定
	ID3D11SamplerState* samplers[] = { m_shadowMapSampler.Get() };
	context->PSSetSamplers(1, 1, samplers);

	auto const viewport = Graphics::GetInstance()->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	ID3D11ShaderResourceView* albedo = s_albedoRT->GetShaderResourceView();
	ID3D11ShaderResourceView* normal = s_normalRT->GetShaderResourceView();
	ID3D11ShaderResourceView* depth = s_depthRT->GetShaderResourceView();
	ID3D11ShaderResourceView* shadow = ShadowMap::GetShadowRenderTexture()->GetShaderResourceView();

	// 定数バッファをマップする
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(s_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer* cb = static_cast<ConstBuffer*>(mappedResource.pData);
	cb->matView = view.Transpose();
	cb->matProj = projection.Transpose();
	cb->inverseViewProj = (view * projection).Invert();
	cb->lightView = ShadowMap::GetLightView().Transpose();
	cb->lightProj = ShadowMap::GetLightProj().Transpose();
	cb->lightNum = PointLightManager::GetPointLights().size();
	for (int i = 0; i < PointLightManager::GetPointLights().size(); i++)
	{
		cb->lightPos[i] = PointLightManager::GetPointLights()[i]->GetPosition();
		cb->lightColor[i] = PointLightManager::GetPointLights()[i]->GetColor();
	}
	// マップを解除する
	context->Unmap(s_constantBuffer.Get(), 0);
	// 定数バッファの設定
	ID3D11Buffer* cbuf[] = { s_constantBuffer.Get() };
	context->VSSetConstantBuffers(1, 1, cbuf);
	context->PSSetConstantBuffers(1, 1, cbuf);
	context->PSSetSamplers(1, 1, m_shadowMapSampler.GetAddressOf());

	// シェーダを設定する
	context->PSSetShaderResources(1, 1, &albedo);
	context->PSSetShaderResources(2, 1, &normal);
	context->PSSetShaderResources(3, 1, &depth);
	context->PSSetShaderResources(4, 1, &shadow);
	context->VSSetShader(s_vertexShader_light.Get(), nullptr, 0);
	context->PSSetShader(s_pixelShader_light.Get(), nullptr, 0);
	context->IASetInputLayout(m_inputLayoutLight.Get());

	s_batch->Begin();
	s_batch->DrawQuad(s_vertex[0], s_vertex[1], s_vertex[3], s_vertex[2]);
	s_batch->End();

	// リソースを使用する前にシェーダーリソーススロットを解除
	ID3D11ShaderResourceView* nullSRV[4] = { nullptr,nullptr,nullptr,nullptr };
	context->PSSetShaderResources(1, 4, nullSRV);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
}

void DeferredRendering::GBufferShow()
{
	using namespace DirectX::SimpleMath;

	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	s_spriteBatch->Begin();
	s_spriteBatch->Draw(s_albedoRT->GetShaderResourceView(), Vector2{0,0}, &rect, DirectX::Colors::White, 0.0f, Vector2::Zero, 0.2f);
	s_spriteBatch->Draw(s_normalRT->GetShaderResourceView(), Vector2{256,0}, &rect, DirectX::Colors::White, 0.0f, Vector2::Zero, 0.2f);
	s_spriteBatch->Draw(s_depthRT->GetShaderResourceView(), Vector2{512,0}, &rect, DirectX::Colors::White, 0.0f, Vector2::Zero, 0.2f);
	s_spriteBatch->End();
}
