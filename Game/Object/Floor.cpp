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
	GetComponent<BoxCollider>()->SetSize({ 100, 1, 100 });
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::Floor);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetFloorModel());
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
	using namespace DirectX::SimpleMath;

	for (int i = 0; i < 2; i++)
	{
		for (int n = 0; n < 2; n++)
		{
			Matrix world = Matrix::CreateTranslation(Vector3{ -50 + float(i) * 100 ,1, -50 + float(n) * 100 } + GetPosition());
			GetComponent<ModelDraw>()->Render(world,true);
		}
	}




	//GetComponent<BoxCollider>()->Render();
}