#include "pch.h"
#include "DropItem.h"
#include "Game/Parts/IPart.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

DropItem::DropItem(IScene* scene, IPart* part)
{
	SetScene(scene);
	AddComponent<BoxCollider>();

	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::DropItem);

	m_menu = std::make_unique<Menu>();
	m_part = part;
	int x;
	int y;
	Graphics::GetInstance()->GetScreenSize(x, y);
	m_menu->Initialize(Graphics::GetInstance()->GetDeviceResources(),x,y);
}

DropItem::~DropItem()
{

}

void DropItem::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
	m_menu->Update();
}

void DropItem::Render()
{
	GetComponent<BoxCollider>().lock().get()->Render();
	if (m_isHit)
	{
		m_menu->Render();
	}
}