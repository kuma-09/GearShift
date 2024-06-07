#include "pch.h"
#include "Player.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Components/MoveRotation.h"
#include "Game/Camera.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/BodyBottom.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"
#include "Game/Particle/Shader.h"



Player::Player()
{
	AddComponent<Move>();
	AddComponent<Camera>();
	AddComponent<BoxCollider>();
	AddComponent<MoveRotation>();
	AddPart<Head>();
	AddPart<BodyTop>();
	AddPart<BodyBottom>();
	AddPart<LeftArm>();
	AddPart<RightArm>();
	AddPart<LeftLeg>();
	AddPart<RightLeg>();
}

Player::~Player()
{

}

void Player::Initialize()
{
	GetComponent<Camera>().lock().get()->SetTarget(this, m_target);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);

	m_shader = std::make_unique<Shader>();
	m_shader->CreateShader();
}

void Player::Update(float elapsedTime)
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

void Player::Render()
{
	auto graphics = Graphics::GetInstance();
	auto states = graphics->GetCommonStates();
	auto context = graphics->GetDeviceResources()->GetD3DDeviceContext();




	GetPart<Head>().lock().get()->Render(GetWorld());
	GetPart<BodyTop>().lock().get()->Render(GetWorld());
	GetPart<BodyBottom>().lock().get()->Render(GetWorld());
	GetPart<LeftArm>().lock().get()->Render(GetWorld());
	GetPart<RightArm>().lock().get()->Render(GetWorld());
	GetPart<LeftLeg>().lock().get()->Render(GetWorld());
	GetPart<RightLeg>().lock().get()->Render(GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();



	Resources::GetInstance()->GetCubeModel()->Draw(context, *states, DirectX::SimpleMath::Matrix::Identity, graphics->GetViewMatrix(), graphics->GetProjectionMatrix(), false, [&]()
		{
			DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateScale(1.2f);
			mat *= GetWorld();

			m_shader->RenderStart(GetWorld(), graphics->GetViewMatrix(), graphics->GetProjectionMatrix());
		}
	);

	//Resources::GetInstance()->GetDiceModel()->Draw(context, *states, GetWorld(), graphics->GetViewMatrix(), graphics->GetProjectionMatrix());

	//Resources::GetInstance()->GetDiceModel()->Draw(context, *states, GetWorld(), graphics->GetViewMatrix(), graphics->GetProjectionMatrix());	Resources::GetInstance()->GetDiceModel()->Draw(context, *states, DirectX::SimpleMath::Matrix::Identity, graphics->GetViewMatrix(), graphics->GetProjectionMatrix(), false, [&]()
	//	{
	//		m_shader->RenderStart(GetWorld(), graphics->GetViewMatrix(), graphics->GetProjectionMatrix());
	//	}
	//);
	
}

void Player::Finalize()
{
	
}

void Player::SetTarget(GameObject* target)
{
	m_target = target;
	GetComponent<Camera>().lock().get()->SetTarget(this, m_target);
}