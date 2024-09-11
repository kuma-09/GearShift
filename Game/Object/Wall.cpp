#include "pch.h"
#include "Wall.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

Wall::Wall(IScene* scene)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Wall);

}

Wall::~Wall()
{
	RemoveAllComponents();
}

void Wall::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(ModelDraw::Cube);
	GetComponent<BoxCollider>()->SetSize(GetScale());
}

void Wall::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	Matrix world = Matrix::Identity;
	world *= Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Wall::Render()
{
	GetComponent<BoxCollider>()->Render();
	GetComponent<ModelDraw>()->Render(GetWorld());
}