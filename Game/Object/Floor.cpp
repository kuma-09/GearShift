#include "pch.h"
#include "Floor.h"
#include "Game/Components/BoxCollider.h"

Floor::Floor(IScene* scene)
{

	SetScene(scene);
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetSize({ 50, 1, 50 });
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::Floor);
	SetPosition({ 0,-1,0 });
}

void Floor::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}

void Floor::Render()
{
	GetComponent<BoxCollider>()->Render();

}