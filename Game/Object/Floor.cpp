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
	SetScale({ 1,1,1 });
	GetComponent<Collider>()->SetInitalePosition({0, -1, 0});
	GetComponent<Collider>()->SetSize({ 300, 1.25f, 300 });
	GetComponent<Collider>()->SetTypeID(Collider::Floor);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetFloorModel(),false);
	SetPosition({ 0,1,0 });
	
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
	SetWorld(world);
	GetComponent<ModelDraw>()->Render();

	//for (int i = 0; i < 2; i++)
	//{
	//    for (int n = 0; n < 2; n++)
	//    {
	//        //Matrix world = Matrix::CreateTranslation(Vector3{ -50 + float(i) * 100 ,1, -50 + float(n) * 100 } + Vector3::Zero );
	//        //Resources::GetInstance()->GetFloorModel()->Draw(context, *state, world, view, proj, false, [&]
	//        //    {
	//        //        Resources::GetInstance()->GetShadow()->Draw(true);
	//        //    }
	//        //);
	//    }
	//}

	//GetComponent<Collider>()->Render();
}