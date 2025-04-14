#include "pch.h"
#include "BurnerVertical.h"
#include "Game/Components/Trail.h"

// �R���X�g���N�^
BurnerVertical::BurnerVertical()
{
	AddComponent<Trail>();
}
// �f�X�g���N�^
BurnerVertical::~BurnerVertical()
{
}
// ����������
void BurnerVertical::Initialize()
{
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10, DirectX::Colors::CornflowerBlue);
}
// �X�V����
void BurnerVertical::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedTime);
	GetComponent<Trail>()->SetPos(GetPosition() - Vector3::Transform(Vector3(0.5f,0, 0),GetQuaternion()), GetPosition() + Vector3::Transform(Vector3(0.5f,0, 0), GetQuaternion()));
}
