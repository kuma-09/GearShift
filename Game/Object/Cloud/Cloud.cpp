#include "pch.h"
#include "Cloud.h"
#include "Game/Components/ModelDraw.h"
// コンストラクタ
Cloud::Cloud(Resources::ModelType modelType)
{
	AddComponent<ModelDraw>();
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(modelType));
}
// デストラクタ
Cloud::~Cloud()
{
}
// 初期化処理
void Cloud::Initialize()
{
}
// 更新処理
void Cloud::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}
