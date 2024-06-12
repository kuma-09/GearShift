#include "pch.h"
#include "DropItem.h"
#include "Game/Components/BoxCollider.h"

DropItem::DropItem()
{
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::DropItem);
}

DropItem::~DropItem()
{

}

void DropItem::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}

void DropItem::Render()
{
	GetComponent<BoxCollider>().lock().get()->Render();
}