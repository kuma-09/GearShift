#include "pch.h"
#include "ModelDraw.h"
#include "Game/GameObject.h"
#include "Game/Shader/Shader.h"

ModelDraw::ModelDraw()
	:
	m_model{}
{
	m_graphics = Graphics::GetInstance();
	m_shader = std::make_unique<Shader>();
	m_shader->CreateShader();
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

void ModelDraw::Render(bool texture, DirectX::XMVECTORF32 color)
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	auto world = GetOwner()->GetWorld();

	m_model->Draw(context, *state, world, view, projection, false, [&]
		{
			Resources::GetInstance()->GetShadow()->Draw(texture, color);
		});
}

void ModelDraw::Render(DirectX::SimpleMath::Matrix world, bool texture, DirectX::XMVECTORF32 color)
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	m_model->Draw(context, *state, world, view, projection, false, [&]
	{
		Resources::GetInstance()->GetShadow()->Draw(texture,color);
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
			Resources::GetInstance()->GetShadow()->RenderDepth();
		});
}


void ModelDraw::OutLineRender()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	auto world = GetOwner()->GetWorld();

	m_model->Draw(context, *state, world, view, projection, false, [&]()
		{
			m_shader->OutlineRenderStart(world, view, projection);
		}
	);
}

void ModelDraw::Finalize()
{

}