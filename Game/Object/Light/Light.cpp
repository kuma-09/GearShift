#include "pch.h"
#include "Light.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/PointLight.h"

Light::Light(IScene* scene)
{
	SetScene(scene);
	AddComponent<ModelDraw>();
	AddComponent<PointLight>();
	SetScale({ 0.2f,0.2f,0.2f });
}

Light::~Light()
{
}

void Light::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetLightModel());
	GetComponent<PointLight>()->Initialize(GetPosition(),{1,0,0});
}

void Light::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedtime);
	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(/*Quaternion::CreateFromYawPitchRoll({DirectX::XMConvertToRadians(-90),DirectX::XMConvertToRadians(45),0}) * */GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Light::Render()
{
}