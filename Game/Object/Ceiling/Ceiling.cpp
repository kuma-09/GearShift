#include "pch.h"
#include "Ceiling.h"
#include "Game/Components/Collider.h"
// �R���X�g���N�^
Ceiling::Ceiling()
{
	AddComponent<Collider>();
}
// �f�X�g���N�^
Ceiling::~Ceiling()
{
}
// ����������
void Ceiling::Initialize()
{
	SetScale({ 300, 1.25f, 300 });
	GetComponent<Collider>()->Initialize(Collider::Floor, Collider::Fixed,GetScale(),{0,-1,0});
}
// �X�V����
void Ceiling::Update(float elapsedtime)
{
	ComponentsUpdate(elapsedtime);
}
// �`�揈��
void Ceiling::Render()
{
}
