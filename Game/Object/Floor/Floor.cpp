#include "pch.h"
#include "Floor.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"

// コンストラクタ
Floor::Floor(Resources::ModelType modelType)
{
	using namespace DirectX::SimpleMath;

	SetScale({ 1,1,1 });

	// コンポーネントを追加
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	GetComponent<Collider>()->Initialize(Collider::Floor,Collider::Fixed, { 300,1.25f,300 },{0,-1,0});
	GetComponent<Collider>()->SetActive(true);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(modelType));

	// 座標を初期化
	SetPosition({ 0,1,0 });
	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

Floor::~Floor()
{
}

// 初期化処理
void Floor::Initialize()
{
}

// 更新処理
void Floor::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
	ComponentsUpdate(elapsedTime);
}

// 描画処理
void Floor::Render()
{
}