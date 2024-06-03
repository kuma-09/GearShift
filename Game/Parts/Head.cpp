#include "pch.h"
#include "Head.h"
#include "Game/Components/BoxCollider.h"

Head::Head()
{
	m_graphics = Graphics::GetInstance();
	m_resources = Resources::GetInstance();
	SetPosition({ 0, 2, 0 });
	AddComponent<BoxCollider>();
}

Head::~Head()
{

}

void Head::Initialize()
{
}

void Head::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	UNREFERENCED_PARAMETER(elapsedTime);

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() + Vector3{ 0, 7, 0 });

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Head::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);


	auto context    = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state      = m_graphics->GetCommonStates();
	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();



	m_resources->GetHeadModel()->Draw(context, *state, GetWorld(), view, projection);
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void Head::Finalize()
{

}