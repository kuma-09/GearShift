#include "pch.h"
#include "Floor.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"

Floor::Floor(IScene* scene)
{
	using namespace DirectX::SimpleMath;

	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetSize({ 50, 1, 50 });
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::Floor);
	GetComponent<ModelDraw>()->Initialize(ModelDraw::Cube);
	SetPosition({ 0,-1,0 });
	
	Matrix world = Matrix::CreateScale({ 50,1,50 });
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Floor::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}

void Floor::Render()
{
	GetComponent<ModelDraw>()->Render(GetWorld());
	GetComponent<BoxCollider>()->Render();

}