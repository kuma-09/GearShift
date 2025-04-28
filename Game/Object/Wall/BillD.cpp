#include "pch.h"
#include "Game/Object/Wall/BillD.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
// コンストラクタ
BillD::BillD(Scene* scene)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
}
// デストラクタ
BillD::~BillD()
{
}
// 初期化処理
void BillD::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::BillC));
	SetScale({ 6, 50, 30 });
	SetQuaternion(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(90), 0, 0));
	GetComponent<Collider>()->Initialize(Collider::Wall,Collider::Fixed, GetScale(), { 0,50,0 });
}
// 更新処理
void BillD::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedtime);
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y,GetPosition().z });
	SetWorld(world);
	ComponentsUpdate(elapsedtime);
}