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
}

void Wall::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(ModelDraw::BillA);
	GetComponent<BoxCollider>()->SetSize(GetScale());
	GetComponent<BoxCollider>()->GetBoundingBox()->Center = GetPosition();
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
	using namespace DirectX::SimpleMath;

	GetComponent<BoxCollider>()->Render();
	GetComponent<ModelDraw>()->Render(Matrix::CreateScale(0.5f) * Matrix::CreateTranslation({0,1,0}));
}