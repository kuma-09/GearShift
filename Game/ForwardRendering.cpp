#include "pch.h"
#include "ForwardRendering.h"
#include "Framework/Graphics.h"

std::unique_ptr<DX::RenderTexture> ForwardRendering::m_renderTexture;

void ForwardRendering::Initialize()
{
	using namespace DirectX::SimpleMath;

	m_renderTexture = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);

	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	m_renderTexture->SetDevice(device);
	m_renderTexture->SetWindow(rect);
}

void ForwardRendering::BeginBuffer()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto depthStencil = Graphics::GetInstance()->GetDeviceResources()->GetDepthStencilView();
	ID3D11RenderTargetView* RTV = m_renderTexture->GetRenderTargetView();

	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto projection = Graphics::GetInstance()->GetProjectionMatrix();

	// -------------------------------------------------------------------------- //
	// レンダーターゲットを変更
	// -------------------------------------------------------------------------- //
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->ClearRenderTargetView(RTV, DirectX::Colors::Black);
	context->OMSetRenderTargets(1, &RTV, depthStencil);
}
