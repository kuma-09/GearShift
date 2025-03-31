#include "pch.h"
#include "Game/Object/Wall/BillA.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
// �R���X�g���N�^
BillA::BillA(IScene* scene)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();

}
// �f�X�g���N�^
BillA::~BillA()
{
}
// ����������
void BillA::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::BillA));
	SetScale({ 10, 8, 6 });
	GetComponent<Collider>()->Initialize(Collider::Wall,Collider::Fixed, GetScale());
}
// �X�V����
void BillA::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedtime);

	Matrix world = Matrix::Identity;
	world *= Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y - GetScale().y,GetPosition().z });
	SetWorld(world);

	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}
