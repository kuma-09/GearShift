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
	SetScale({ 50, 5, 50 });
	GetComponent<Collider>()->Initialize(Collider::Wall, Collider::Fixed,GetScale(),{0,5,0});
}

void Ceiling::Update(float elapsedtime)
{
	ComponentsUpdate(elapsedtime);
}

void Ceiling::Render()
{
}
