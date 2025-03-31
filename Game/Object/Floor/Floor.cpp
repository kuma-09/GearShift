#include "pch.h"
#include "Floor.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"

// �R���X�g���N�^
Floor::Floor(Resources::ModelType modelType)
{
	using namespace DirectX::SimpleMath;

	SetScale({ 1,1,1 });

	// �R���|�[�l���g��ǉ�
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	GetComponent<Collider>()->Initialize(Collider::Floor,Collider::Fixed, { 300,1.25f,300 },{0,-1,0});
	GetComponent<Collider>()->SetActive(true);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(modelType));

	// ���W��������
	SetPosition({ 0,1,0 });
	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

Floor::~Floor()
{
}

// ����������
void Floor::Initialize()
{
}

// �X�V����
void Floor::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
	ComponentsUpdate(elapsedTime);
}

// �`�揈��
void Floor::Render()
{
}