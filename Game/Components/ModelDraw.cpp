#include "pch.h"
#include "ModelDraw.h"
#include "Game/Particle/Shader.h"

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

void ModelDraw::Render(ModelType type, DirectX::SimpleMath::Matrix world)
{

	auto context    = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state      = m_graphics->GetCommonStates();
	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	switch (type)
	{
	case ModelDraw::Head:
		m_resources->GetHeadModel()->Draw(context, *state, world, view, projection);
		break;
	case ModelDraw::BodyTop:
		m_resources->GetBodyTopModel()->Draw(context, *state, world, view, projection);
		break;
	case ModelDraw::BodyBottom:
		m_resources->GetBodyBottomModel()->Draw(context, *state, world, view, projection);
		break;
	case ModelDraw::LArm:
		m_resources->GetlArmModel()->Draw(context, *state, world, view, projection);
		break;
	case ModelDraw::RArm:
		m_resources->GetrArmModel()->Draw(context, *state, world, view, projection);
		break;
	case ModelDraw::LLeg:
		m_resources->GetlLegModel()->Draw(context, *state, world, view, projection);
		break;
	case ModelDraw::RLeg:
		m_resources->GetrLegModel()->Draw(context, *state, world, view, projection);
		break;
	case ModelDraw::Dice:
		m_resources->GetDiceModel()->Draw(context, *state, world, view, projection);
		break;
	default:
		break;
	}

}

void ModelDraw::Finalize()
{

}