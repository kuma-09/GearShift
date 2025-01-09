#include "pch.h"
#include "Floor2.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"

Floor2::Floor2(IScene* scene)
{
	using namespace DirectX::SimpleMath;

	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	SetScale({ 1,1,1 });
	GetComponent<Collider>()->Initialize(Collider::Floor, { 300,1.25f,300 }, { 0,-1,0 });
	GetComponent<Collider>()->SetActive(true);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetFloor2Model(), false);
	SetPosition({ 0,1,0 });
	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Floor2::Initialize()
{
}

void Floor2::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
	ComponentsUpdate(elapsedTime);
}

void Floor2::Render()
{
}
