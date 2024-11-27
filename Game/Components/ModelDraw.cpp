#include "pch.h"
#include "ModelDraw.h"
#include "Game/GameObject.h"
#include "Game/Shader/Shader.h"
#include "Game/DeferredRendering.h"

ModelDraw::ModelDraw()
	:
	m_model{}
{
	m_graphics = Graphics::GetInstance();
}

ModelDraw::~ModelDraw()
{
	Finalize();
}

void ModelDraw::Initialize(DirectX::Model* model)
{
	m_model = model;
}

void ModelDraw::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	
}

void ModelDraw::Render(DirectX::SimpleMath::Matrix world, bool texture, DirectX::XMVECTORF32 color)
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	m_model->Draw(context, *state, world, view, projection, false, [&]
	{
		//Resources::GetInstance()->GetShadow()->Draw(texture,color);
		DeferredRendering::DrawGBuffer(texture);
	});
}

void ModelDraw::CreateShadow(DirectX::SimpleMath::Matrix world)
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	//m_model->Draw(context, *state, world, view, projection, false, [&]
	//	{
	//		Resources::GetInstance()->GetShadow()->RenderDepth();
	//	});
}

void ModelDraw::Finalize()
{

}