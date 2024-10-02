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

void ModelDraw::Initialize(ModelType type)
{
	Resources* resources = Resources::GetInstance();

	switch (type)
	{
	case ModelDraw::Player:
		m_model = resources->GetPlayerModel();
	case ModelDraw::Head:
		m_model = resources->GetHeadModel();
		break;
	case ModelDraw::BodyTop:
		m_model = resources->GetBodyTopModel();
		break;
	case ModelDraw::BodyBottom:
		m_model = resources->GetBodyBottomModel();
		break;
	case ModelDraw::LArm:
		m_model = resources->GetlArmModel();
		break;
	case ModelDraw::RArm:
		m_model = resources->GetrArmModel();
		break;
	case ModelDraw::LLeg:
		m_model = resources->GetlLegModel();
		break;
	case ModelDraw::RLeg:
		m_model = resources->GetrLegModel();
		break;
	case ModelDraw::Dice:
		m_model = resources->GetDiceModel();
		break;
	case ModelDraw::Cube:
		m_model = resources->GetCubeModel();
		break;
	case ModelDraw::Floor:
		m_model = resources->GetFloorModel();
		break;
	case ModelDraw::BillA:
		m_model = resources->GetBillAModel();
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

void ModelDraw::Render(DirectX::SimpleMath::Matrix world)
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	m_model->Draw(context, *state, world, view, projection);

}

void ModelDraw::Render(DirectX::SimpleMath::Matrix world, DirectX::XMVECTORF32 color)
{

	auto context    = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state      = m_graphics->GetCommonStates();
	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	m_model->Draw(context, *state, world, view, projection, false, [&]()
	{
		SetColor(color);
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

void ModelDraw::SetColor(DirectX::XMVECTORF32 color)
{
	if (!m_effect)
	{
		m_effect = std::make_unique<DirectX::BasicEffect>(m_graphics->GetDeviceResources()->GetD3DDevice());
	}

	m_effect->SetDiffuseColor(color);
	m_effect->SetView(m_graphics->GetViewMatrix());
	m_effect->SetProjection(m_graphics->GetProjectionMatrix());
	m_effect->SetWorld(GetOwner()->GetWorld());
	m_effect->Apply(m_graphics->GetDeviceResources()->GetD3DDeviceContext());
}