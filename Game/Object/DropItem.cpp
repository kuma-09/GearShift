#include "pch.h"
#include "DropItem.h"
#include "Framework/Audio.h"
#include "Game/Components/Physics.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HP.h"
#include "Game/Manager/ObjectManager.h"
// コンストラクタ
DropItem::DropItem(Scene* scene)
{
	SetScene(scene);
	SetScale({ MODEL_SIZE,MODEL_SIZE,MODEL_SIZE });
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
}
// デストラクタ
DropItem::~DropItem()
{
}
// 初期化処理
void DropItem::Initialize()
{
	using namespace DirectX::SimpleMath;
	GetComponent<Physics>()->Initialize();
	GetComponent<Collider>()->Initialize(Collider::DropItemB,Collider::Collision, { 2,2,2 });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::DropItem), true);
	GetComponent<ModelDraw>()->SetEmissiveColor({ 1,0.5f,0.5f,0.5f });

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}
// 更新処理
void DropItem::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetQuaternion(GetQuaternion() * Quaternion::CreateFromYawPitchRoll({ 0,1 * 3.14 / 180,0 }));
	SetPosition(GetPosition() + GetVelocity() * elapsedTime);

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}
// 当たり判定の処理
void DropItem::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::Floor ||
		collider->GetTypeID() == Collider::Wall)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}
	if (collider->GetTypeID() == Collider::Player)
	{
		// プレイヤーのHPを回復
		collider->GetOwner()->GetComponent<HP>()->SetHP(collider->GetOwner()->GetComponent<HP>()->GetHP() + RECOVERY_HP_POINT);
		Audio::GetInstance()->PlaySoundSE_PowerUp();
		ObjectManager::Remove(this);
	}
}
