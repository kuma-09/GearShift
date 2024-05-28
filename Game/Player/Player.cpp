#include "pch.h"
#include "Player.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Camera.h"
#include "Game/Parts/Head.h"

Player::Player()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
    AddComponent<Move>();
	AddPart<Head>();
}

Player::~Player()
{

}

void Player::Initialize()
{
}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);
	PartUpdate(elapsedTime);

	Matrix world = Matrix::Identity;
	world =  Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Player::Render()
{
	GetComponent<ModelDraw>().lock().get()->Render(GetWorld());
	GetComponent<BoxCollider>().lock().get()->Render();
	GetPart<Head>().lock().get()->Render(GetWorld());
}

void Player::Finalize()
{
	
}