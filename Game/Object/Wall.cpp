#include "pch.h"
#include "Wall.h"
#include "Game/Components/BoxCollider.h"

Wall::Wall(IScene* scene)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Wall);
}

Wall::~Wall()
{

}

void Wall::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}

void Wall::Render()
{
	GetComponent<BoxCollider>().lock().get()->Render();
}