#include "pch.h"
#include "BurnerHorizon.h"
#include "Game/Components/Trail.h"

// �R���X�g���N�^
BurnerHorizon::BurnerHorizon()
{
	AddComponent<Trail>();
}
// �f�X�g���N�^
BurnerHorizon::~BurnerHorizon()
{
}
// ����������
void BurnerHorizon::Initialize()
{
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10, DirectX::Colors::CornflowerBlue);
}
// �X�V����
void BurnerHorizon::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	GetComponent<Trail>()->SetPos(GetPosition() - Vector3(0,0.5f, 0), GetPosition() + Vector3(0,0.5f, 0));
}
