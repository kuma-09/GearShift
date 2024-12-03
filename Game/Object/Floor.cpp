#include "pch.h"
#include "Floor.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"

Floor::Floor(IScene* scene)
{
	using namespace DirectX::SimpleMath;

	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	SetScale({ 6,1,6 });
	GetComponent<Collider>()->SetSize({ 300, 1, 300 });
	GetComponent<Collider>()->SetTypeID(Collider::Floor);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetFloorModel());
	SetPosition({ 0,-1,0 });
	
	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Floor::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}

void Floor::Render()
{
	using namespace DirectX::SimpleMath;

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	GetComponent<ModelDraw>()->Render(world,true);
	GetComponent<Collider>()->Render();
}