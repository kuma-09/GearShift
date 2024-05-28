#include "pch.h"
#include "ModelDraw.h"

ModelDraw::ModelDraw()
{
	m_graphics = Graphics::GetInstance();
	m_resources = Resources::GetInstance();
}

ModelDraw::~ModelDraw()
{

}

void ModelDraw::Initialize()
{

}

void ModelDraw::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

void ModelDraw::Render(DirectX::SimpleMath::Matrix world)
{

	auto context    = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state      = m_graphics->GetCommonStates();
	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	m_resources->GetModel()->Draw(context, *state, world, view, projection);
}

void ModelDraw::Finalize()
{

}