#include "pch.h"
#include "ModelDraw.h"
#include "Game/GameObject.h"
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

void ModelDraw::Render(ModelType type, DirectX::SimpleMath::Matrix world,bool black)
{

	auto context    = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state      = m_graphics->GetCommonStates();
	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	if (black)
	{
		switch (type)
		{
		case ModelDraw::Player:
			m_resources->GetPlayerModel()->Draw(context, *state, world, view, projection, false, [&]()
				{
					SetBlack();
				});
		case ModelDraw::Head:
			m_resources->GetHeadModel()->Draw(context, *state, world, view, projection, false, [&]()
				{
					SetBlack();
				});
			break;
		case ModelDraw::BodyTop:
			m_resources->GetBodyTopModel()->Draw(context, *state, world, view, projection, false, [&]()
				{
					SetBlack();
				});
			break;
		case ModelDraw::BodyBottom:
			m_resources->GetBodyBottomModel()->Draw(context, *state, world, view, projection, false, [&]()
				{
					SetBlack();
				});
			break;
		case ModelDraw::LArm:
			m_resources->GetlArmModel()->Draw(context, *state, world, view, projection, false, [&]()
				{
					SetBlack();
				});
			break;
		case ModelDraw::RArm:
			m_resources->GetrArmModel()->Draw(context, *state, world, view, projection, false, [&]()
				{
					SetBlack();
				});
			break;
		case ModelDraw::LLeg:
			m_resources->GetlLegModel()->Draw(context, *state, world, view, projection, false, [&]()
				{
					SetBlack();
				});
			break;
		case ModelDraw::RLeg:
			m_resources->GetrLegModel()->Draw(context, *state, world, view, projection, false, [&]()
				{
					SetBlack();
				});
			break;
		case ModelDraw::Dice:
			m_resources->GetDiceModel()->Draw(context, *state, world, view, projection);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (type)
		{
		case ModelDraw::Player:
			m_resources->GetPlayerModel()->Draw(context, *state, world, view, projection);
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



}

void ModelDraw::Finalize()
{

}

void ModelDraw::SetBlack()
{
	auto effect = std::make_unique<DirectX::BasicEffect>(m_graphics->GetDeviceResources()->GetD3DDevice());
	effect->SetDiffuseColor(DirectX::SimpleMath::Vector4(0, 0, 0, 1));
	effect->SetView(m_graphics->GetViewMatrix());
	effect->SetProjection(m_graphics->GetProjectionMatrix());
	effect->SetWorld(GetOwner()->GetWorld());
	effect->Apply(m_graphics->GetDeviceResources()->GetD3DDeviceContext());
}