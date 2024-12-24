#include "pch.h"	
#include "Gun.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"

Gun::Gun(GameObject* owner)
{
	m_owner = owner;
	AddComponent<ModelDraw>();
}

Gun::~Gun()
{
}

void Gun::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetGunModel());
}

void Gun::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetQuaternion(Quaternion::CreateFromYawPitchRoll(0, DirectX::XMConvertToRadians(-135),0) * m_owner->GetQuaternion());
	SetPosition(m_owner->GetPosition() + Vector3::Transform(Vector3(0.2f,1.0f,-0.25f), GetQuaternion()));
	//SetPosition(m_owner->GetPosition() + Vector3::Transform(pos, quaternion));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}
