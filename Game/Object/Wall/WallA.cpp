#include "pch.h"
#include "Game/Object/Wall/WallA.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
// �R���X�g���N�^
WallA::WallA(Scene* scene)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
}
// �f�X�g���N�^
WallA::~WallA()
{
}
// ����������
void WallA::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::BillB));
	SetScale({ 4, 8, 4 });
	GetComponent<Collider>()->Initialize(Collider::Wall,Collider::Fixed, GetScale());
	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}
// �X�V����
void WallA::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedtime);
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y - GetScale().y,GetPosition().z });
	SetWorld(world);

	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}