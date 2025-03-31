#include "pch.h"
#include "Light.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/PointLight.h"
// コンストラクタ
Light::Light(IScene* scene)
{
	SetScene(scene);
	AddComponent<ModelDraw>();
	AddComponent<PointLight>();
	SetScale({ 0.2f,0.2f,0.2f });
}
// デストラクタ
Light::~Light()
{
}
// 初期化処理
void Light::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Light));
	GetComponent<PointLight>()->Initialize(GetPosition(),{1,0,0});
}
// 更新処理
void Light::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedtime);
	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}
// 描画処理
void Light::Render()
{
}