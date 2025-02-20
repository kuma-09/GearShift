#include "pch.h"
#include "ModelDraw.h"
#include "Game/GameObject.h"
#include "Game/Shader/Shader.h"
#include "Game/DeferredRendering.h"
#include "Game/Manager/RenderManager.h"
#include "Game/Shader/ShadowMap.h"

ModelDraw::ModelDraw()
	:
	m_model{},
	m_isTexture{},
	m_isRimLight{},
	m_rimColor{}
{
	m_graphics = Graphics::GetInstance();
}

ModelDraw::~ModelDraw()
{
	RenderManager::Remove(this);
}

void ModelDraw::Initialize(DirectX::Model* model,bool texture)
{
	m_model = model;
	m_isTexture = texture;
	RenderManager::Add(this);
}

void ModelDraw::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

void ModelDraw::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	auto world = GetOwner()->GetWorld();

	m_model->Draw(context, *state, world, view, projection, false, [&]
	{
		DeferredRendering::DrawGBuffer(m_isTexture,m_isRimLight,m_rimColor);
	});
}

void ModelDraw::CreateShadow()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	auto world = GetOwner()->GetWorld();

	m_model->Draw(context, *state, world, view, projection, false, [&]
		{
			ShadowMap::RenderDepth();
		});
}

void ModelDraw::SetRimLithgColor(DirectX::XMVECTORF32 color)
{
	m_isRimLight = true;
	m_rimColor = color;
}
