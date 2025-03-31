#include "pch.h"
#include "SkyDome.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"

// �R���X�g���N�^
SkyDome::SkyDome()
{
	using namespace DirectX::SimpleMath;
	SetWorld(Matrix::Identity);
	AddComponent<ModelDraw>();
}

// �f�X�g���N�^
SkyDome::~SkyDome()
{
}

// ����������
void SkyDome::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::SkyDome), true);
	GetComponent<ModelDraw>()->SetEmissiveColor({0.25f,0.25f,0.25f,1.0f});
}

// �X�V����
void SkyDome::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}
