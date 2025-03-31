#include "pch.h"
#include "Light.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/PointLight.h"
// �R���X�g���N�^
Light::Light(IScene* scene)
{
	SetScene(scene);
	AddComponent<ModelDraw>();
	AddComponent<PointLight>();
	SetScale({ 0.2f,0.2f,0.2f });
}
// �f�X�g���N�^
Light::~Light()
{
}
// ����������
void Light::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Light));
	GetComponent<PointLight>()->Initialize(GetPosition(),{1,0,0});
}
// �X�V����
void Light::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedtime);
	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}
// �`�揈��
void Light::Render()
{
}