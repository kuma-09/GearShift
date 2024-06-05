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

	//	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	Shader::ConstBuffer cbuff;
	//	ビュー設定
	cbuff.matView = view.Transpose();
	//	プロジェクション設定
	cbuff.matProj = projection.Transpose();
	//	ワールド設定
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	//	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(Shader::GetBuffer().Get(), 0, NULL, &cbuff, 0, 0);

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