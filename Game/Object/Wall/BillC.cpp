#include "pch.h"
#include "Game/Object/Wall/BillC.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
// コンストラクタ
BillC::BillC(Scene* scene)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
}
// デストラクタ
BillC::~BillC()
{
}
// 初期化処理
void BillC::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::BillC));
	SetScale({ 30, 50, 6 });
	GetComponent<Collider>()->Initialize(Collider::Wall,Collider::Fixed, GetScale(),{0,50,0});
}
// 更新処理
void BillC::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedtime);
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y,GetPosition().z });
	SetWorld(world);
	ComponentsUpdate(elapsedtime);
}
