#include "pch.h"
#include "ModelDraw.h"
#include "Game/GameObject.h"
#include "Game/Particle/Shader.h"

ModelDraw::ModelDraw()
	:
	m_model{}
{
	m_graphics = Graphics::GetInstance();
	m_resources = Resources::GetInstance();
}

ModelDraw::~ModelDraw()
{

}

void ModelDraw::Initialize(ModelType type)
{
	switch (type)
	{
	case ModelDraw::Player:
		m_model = m_resources->GetPlayerModel();
	case ModelDraw::Head:
		m_model = m_resources->GetHeadModel();
		break;
	case ModelDraw::BodyTop:
		m_model = m_resources->GetBodyTopModel();
		break;
	case ModelDraw::BodyBottom:
		m_model = m_resources->GetBodyBottomModel();
		break;
	case ModelDraw::LArm:
		m_model = m_resources->GetlArmModel();
		break;
	case ModelDraw::RArm:
		m_model = m_resources->GetrArmModel();
		break;
	case ModelDraw::LLeg:
		m_model = m_resources->GetlLegModel();
		break;
	case ModelDraw::RLeg:
		m_model = m_resources->GetrLegModel();
		break;
	case ModelDraw::Dice:
		m_model = m_resources->GetDiceModel();
		break;
	default:
		m_model = nullptr;
		break;
	}
}

void ModelDraw::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	
}

void ModelDraw::Render(DirectX::SimpleMath::Matrix world,bool black)
{

	auto context    = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state      = m_graphics->GetCommonStates();
	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	

	if (black)
	{
		m_model->Draw(context, *state, world, view, projection, false, [&]()
		{
			SetBlack();
		});
	}
	else
	{
		m_model->Draw(context, *state, world, view, projection);
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