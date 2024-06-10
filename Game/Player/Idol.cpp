#include "pch.h"
#include "Idol.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Gravity.h"
#include "Game/Camera.h"
#include "Game/Player/Emitter.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/BodyBottom.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"
#include "Game/Particle/Shader.h"



Idol::Idol()
{
	AddComponent<Move>();
	AddComponent<Camera>();
	AddComponent<BoxCollider>();
	AddComponent<Gravity>();
	AddComponent<Emitter>();
	AddPart<Head>();
	AddPart<BodyTop>();
	AddPart<BodyBottom>();
	AddPart<LeftArm>();
	AddPart<RightArm>();
	AddPart<LeftLeg>();
	AddPart<RightLeg>();
}

Idol::~Idol()
{

}

void Idol::Initialize(GameObject* gameobject)
{
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void Idol::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);
	PartUpdate(elapsedTime);

	SetPosition(GetPosition() + GetVelocity());

	Matrix world = Matrix::Identity;
	world =  Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Idol::Render()
{
	auto graphics = Graphics::GetInstance();
	auto states = graphics->GetCommonStates();
	auto context = graphics->GetDeviceResources()->GetD3DDeviceContext();



	GetComponent<Emitter>().lock().get()->Render(GetPosition());
	GetPart<Head>().lock().get()->Render(GetWorld());
	GetPart<BodyTop>().lock().get()->Render(GetWorld());
	GetPart<BodyBottom>().lock().get()->Render(GetWorld());
	GetPart<LeftArm>().lock().get()->Render(GetWorld());
	GetPart<RightArm>().lock().get()->Render(GetWorld());
	GetPart<LeftLeg>().lock().get()->Render(GetWorld());
	GetPart<RightLeg>().lock().get()->Render(GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
	
}

void Idol::Finalize()
{
	
}

void Idol::SetTarget(GameObject* target)
{
	m_target = target;
	GetComponent<Camera>().lock().get()->SetTarget(this, m_target);
}