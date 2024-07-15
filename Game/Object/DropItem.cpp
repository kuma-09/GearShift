#include "pch.h"
#include "DropItem.h"
#include "Game/Parts/IPart.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

DropItem::DropItem(IScene* scene, std::unique_ptr<IPart> part)
{


	SetScene(scene);
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::DropItem);
	AddComponent<ModelDraw>();
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::LLeg);

	m_menu = std::make_unique<Menu>();
	m_part = std::move(part);



}

DropItem::~DropItem()
{

}

void DropItem::Initialize()
{
	using namespace DirectX::SimpleMath;

	int x;
	int y;
	Graphics::GetInstance()->GetScreenSize(x, y);
	m_menu->Initialize(Graphics::GetInstance()->GetDeviceResources(), x, y);

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItem::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);
	m_menu->Update();

	SetQuaternion(GetQuaternion() * Quaternion::CreateFromYawPitchRoll({ 0,1 * 3.14 / 180,0 }));

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItem::Render()
{
	//GetComponent<BoxCollider>().lock().get()->Render();
	GetComponent<ModelDraw>().lock().get()->Render(GetWorld());
	if (m_isHit)
	{
		m_menu->Render();
	}
}