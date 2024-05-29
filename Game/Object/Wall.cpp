#include "pch.h"
#include "Wall.h"
#include "Game/Components/BoxCollider.h"

Wall::Wall()
{
	AddComponent<BoxCollider>();
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