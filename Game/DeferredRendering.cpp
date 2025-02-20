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

// �ÓI�����o�ϐ��̏�����
Graphics* DeferredRendering::s_graphics;
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
Microsoft::WRL::ComPtr<ID3D11Buffer> DeferredRendering::s_constantBuffer_rim;
Microsoft::WRL::ComPtr<ID3D11SamplerState> DeferredRendering::m_shadowMapSampler;
DirectX::SimpleMath::Matrix  DeferredRendering::s_lightViewProj;
DirectX::SimpleMath::Vector3 DeferredRendering::s_lightPosition;

// �C���v�b�g���C�A�E�g
Microsoft::WRL::ComPtr<ID3D11InputLayout> DeferredRendering::s_inputLayoutGBuffer;
Microsoft::WRL::ComPtr<ID3D11InputLayout> DeferredRendering::m_inputLayoutLight;
// �X�v���C�g�o�b�`
std::unique_ptr<DirectX::SpriteBatch> DeferredRendering::s_spriteBatch;
// �v���~�e�B�u�o�b�`
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> DeferredRendering::s_batch;
// ���_���
DirectX::VertexPositionTexture DeferredRendering::s_vertex[4];

void DeferredRendering::Initialize()
{
	using namespace DirectX::SimpleMath;

	s_graphics = Graphics::GetInstance();

	s_albedoRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);
	s_normalRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);
	s_depthRT  = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	s_finalRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);

	auto device = s_graphics->GetDeviceResources()->GetD3DDevice();
	auto context = s_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto rect = s_graphics->GetDeviceResources()->GetOutputSize();

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

	//	�C���v�b�g���C�A�E�g�̍쐬
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

	// �萔�o�b�t�@�p�̃o�b�t�@�I�u�W�F�N�g���쐬����
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstBuffer));	// 16�̔{�����w�肷��
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(
		device->CreateBuffer(&bufferDesc, nullptr, s_constantBuffer.ReleaseAndGetAddressOf())
	);

	// �萔�o�b�t�@�p�̃o�b�t�@�I�u�W�F�N�g���쐬����
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstBuffer_RimLight));	// 16�̔{�����w�肷��
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DX::ThrowIfFailed(
		device->CreateBuffer(&bufferDesc, nullptr, s_constantBuffer_rim.ReleaseAndGetAddressOf())
	);

	// �X�v���C�g�o�b�`�̐���
	s_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// �v���~�e�B�u�o�b�`�̍쐬
	s_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	s_vertex[0] = { Vector3(-1.0f , 1.0f,0) , Vector2(0, 0) };	//����
	s_vertex[1] = { Vector3( 1.0f , 1.0f,0) , Vector2(1, 0) };	//�E��
	s_vertex[2] = { Vector3(-1.0f ,-1.0f,0) , Vector2(0, 1) };	//����
	s_vertex[3] = { Vector3( 1.0f ,-1.0f,0) , Vector2(1, 1) };	//�E��

	// �T���v���[�̍쐬�i�V���h�E�}�b�v�p�j
	D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&sampler_desc, m_shadowMapSampler.ReleaseAndGetAddressOf());
}

void DeferredRendering::BeginGBuffer()
{
	auto rect = s_graphics->GetDeviceResources()->GetOutputSize();

	s_albedoRT->SetWindow(rect);
	s_normalRT->SetWindow(rect);
	s_depthRT->SetWindow(rect);
	s_finalRT->SetWindow(rect);

	auto context = s_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states  = s_graphics->GetCommonStates();
	auto depthStencil = s_graphics->GetDeviceResources()->GetDepthStencilView();
	auto albedoRTV = s_albedoRT->GetRenderTargetView();
	auto normalRTV = s_normalRT->GetRenderTargetView();
	auto depthRTV = s_depthRT->GetRenderTargetView();
	auto view = s_graphics->GetViewMatrix();
	auto projection = s_graphics->GetProjectionMatrix();

	ID3D11RenderTargetView* renderTargets[3] = { albedoRTV, normalRTV, depthRTV};
	// -------------------------------------------------------------------------- //
	// �����_�[�^�[�Q�b�g��ύX
	// -------------------------------------------------------------------------- //
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	for (auto& rtv : renderTargets)
	{
		context->ClearRenderTargetView(rtv, DirectX::Colors::Black);
	}
	context->OMSetRenderTargets(3, renderTargets, depthStencil);

	// �萔�o�b�t�@���}�b�v����
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(s_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer* cb = static_cast<ConstBuffer*>(mappedResource.pData);
	cb->matView = view.Transpose();
	cb->matProj = projection.Transpose();
	cb->inverseViewProj = (view * projection).Invert();
	// �}�b�v����������
	context->Unmap(s_constantBuffer.Get(), 0);
	// �萔�o�b�t�@�̐ݒ�
	ID3D11Buffer* cbuf[] = { s_constantBuffer.Get() };
	context->VSSetConstantBuffers(1, 1, cbuf);
	context->PSSetConstantBuffers(1, 1, cbuf);
}

void DeferredRendering::DrawGBuffer(bool texture,bool rim, DirectX::XMVECTORF32 rimColor)
{
	auto context = s_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = s_graphics->GetCommonStates();
	auto view = s_graphics->GetViewMatrix();
	auto projection = s_graphics->GetProjectionMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map(s_constantBuffer_rim.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer_RimLight* cbRim = static_cast<ConstBuffer_RimLight*>(mappedResource.pData);
	cbRim->isRim = rim;
	cbRim->rimColor = rimColor;
	// �}�b�v����������
	context->Unmap(s_constantBuffer_rim.Get(), 0);
	ID3D11Buffer* cbuf[] = { s_constantBuffer_rim.Get() };
	context->PSSetConstantBuffers(2, 1, cbuf);

	// �V�F�[�_��ݒ肷��
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
	auto context = s_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto device = s_graphics->GetDeviceResources()->GetD3DDevice();
	auto renderTarget = s_finalRT->GetRenderTargetView();
	auto depthStencil = s_graphics->GetDeviceResources()->GetDepthStencilView();
	auto view = s_graphics->GetViewMatrix();
	auto projection = s_graphics->GetProjectionMatrix();
	auto states = s_graphics->GetCommonStates();

	//renderTarget = Graphics::GetInstance()->GetDeviceResources()->GetRenderTargetView();
	//context->ClearRenderTargetView(renderTarget, DirectX::Colors::Black);
	context->OMSetRenderTargets(1, &renderTarget, nullptr);
	context->RSSetState(states->CullNone());						// �J�����O

	// �e�N�X�`���T���v���[�̐ݒ�
	ID3D11SamplerState* samplers[] = { m_shadowMapSampler.Get() };
	context->PSSetSamplers(1, 1, samplers);
	
	auto const viewport = s_graphics->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	ID3D11ShaderResourceView* albedo = s_albedoRT->GetShaderResourceView();
	ID3D11ShaderResourceView* normal = s_normalRT->GetShaderResourceView();
	ID3D11ShaderResourceView* depth = s_depthRT->GetShaderResourceView();
	ID3D11ShaderResourceView* shadow0 = ShadowMap::GetShadowRenderTexture(0)->GetShaderResourceView();
	ID3D11ShaderResourceView* shadow1 = ShadowMap::GetShadowRenderTexture(1)->GetShaderResourceView();
	ID3D11ShaderResourceView* shadow2 = ShadowMap::GetShadowRenderTexture(2)->GetShaderResourceView();
	ID3D11ShaderResourceView* shadow3 = ShadowMap::GetShadowRenderTexture(3)->GetShaderResourceView();

	// �萔�o�b�t�@���}�b�v����
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(s_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ConstBuffer* cb = static_cast<ConstBuffer*>(mappedResource.pData);
	cb->matView = view.Transpose();
	cb->matProj = projection.Transpose();
	cb->inverseViewProj = (view * projection).Invert();
	cb->lightView[0] = ShadowMap::GetLightView(0).Transpose();
	cb->lightView[1] = ShadowMap::GetLightView(1).Transpose();
	cb->lightView[2] = ShadowMap::GetLightView(2).Transpose();
	cb->lightView[3] = ShadowMap::GetLightView(3).Transpose();
	cb->lightProj[0] = ShadowMap::GetLightProj(0).Transpose();
	cb->lightProj[1] = ShadowMap::GetLightProj(1).Transpose();
	cb->lightProj[2] = ShadowMap::GetLightProj(2).Transpose();
	cb->lightProj[3] = ShadowMap::GetLightProj(3).Transpose();
	cb->lightNum = PointLightManager::GetPointLights().size();
	for (int i = 0; i < PointLightManager::GetPointLights().size(); i++)
	{
		cb->lightPos[i] = PointLightManager::GetPointLights()[i]->GetPosition();
		cb->lightColor[i] = PointLightManager::GetPointLights()[i]->GetColor();
	}
	// �}�b�v����������
	context->Unmap(s_constantBuffer.Get(), 0);

	// �萔�o�b�t�@�̐ݒ�
	ID3D11Buffer* cbuf[] = { s_constantBuffer.Get()};
	context->VSSetConstantBuffers(1, 1, cbuf);
	context->PSSetConstantBuffers(1, 1, cbuf);
	context->PSSetSamplers(1, 1, m_shadowMapSampler.GetAddressOf());

	// �V�F�[�_��ݒ肷��
	context->PSSetShaderResources(1, 1, &albedo);
	context->PSSetShaderResources(2, 1, &normal);
	context->PSSetShaderResources(3, 1, &depth);
	context->PSSetShaderResources(4, 1, &shadow0);
	context->PSSetShaderResources(5, 1, &shadow1);
	context->PSSetShaderResources(6, 1, &shadow2);
	context->PSSetShaderResources(7, 1, &shadow3);
	context->VSSetShader(s_vertexShader_light.Get(), nullptr, 0);
	context->PSSetShader(s_pixelShader_light.Get(), nullptr, 0);
	context->IASetInputLayout(m_inputLayoutLight.Get());

	s_batch->Begin();
	s_batch->DrawQuad(s_vertex[0], s_vertex[1], s_vertex[3], s_vertex[2]);
	s_batch->End();

	// ���\�[�X���g�p����O�ɃV�F�[�_�[���\�[�X�X���b�g������
	ID3D11ShaderResourceView* nullSRV[4] = { nullptr,nullptr,nullptr,nullptr };
	context->PSSetShaderResources(1, 4, nullSRV);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
}

void DeferredRendering::GBufferShow()
{
	using namespace DirectX::SimpleMath;

	RECT rect = s_graphics->GetDeviceResources()->GetOutputSize();

	s_spriteBatch->Begin();
	s_spriteBatch->Draw(s_albedoRT->GetShaderResourceView(), Vector2{0,0}, &rect, DirectX::Colors::White, 0.0f, Vector2::Zero, 0.2f);
	s_spriteBatch->Draw(s_normalRT->GetShaderResourceView(), Vector2{256,0}, &rect, DirectX::Colors::White, 0.0f, Vector2::Zero, 0.2f);
	s_spriteBatch->Draw(s_depthRT->GetShaderResourceView(), Vector2{512,0}, &rect, DirectX::Colors::White, 0.0f, Vector2::Zero, 0.2f);
	s_spriteBatch->End();
}
