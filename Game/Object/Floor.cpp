#include "pch.h"
#include "Floor.h"
#include "Game/Components/BoxCollider.h"

Floor::Floor()
{
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetSize({ 50, 5, 50 });
	SetPosition({ 0,0,0 });
}

void Floor::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}

void Floor::Render()
{
	GetComponent<BoxCollider>()->Render();
}