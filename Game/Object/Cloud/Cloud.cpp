#include "pch.h"
#include "Cloud.h"
#include "Game/Components/ModelDraw.h"
// �R���X�g���N�^
Cloud::Cloud(Resources::ModelType modelType)
{
	AddComponent<ModelDraw>();
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(modelType));
}
// �f�X�g���N�^
Cloud::~Cloud()
{
}
// ����������
void Cloud::Initialize()
{
}
// �X�V����
void Cloud::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}
