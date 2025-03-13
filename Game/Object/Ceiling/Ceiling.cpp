#include "pch.h"
#include "Ceiling.h"
#include "Game/Components/Collider.h"

Ceiling::Ceiling()
{
	AddComponent<Collider>();
}

Ceiling::~Ceiling()
{
}

void Ceiling::Initialize()
{
	SetScale({ 300, 1.25f, 300 });
	GetComponent<Collider>()->Initialize(Collider::Floor, Collider::Fixed,GetScale(),{0,-1,0});
}

void Ceiling::Update(float elapsedtime)
{
	ComponentsUpdate(elapsedtime);
}

void Ceiling::Render()
{
}
