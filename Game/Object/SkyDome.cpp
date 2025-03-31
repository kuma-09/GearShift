#include "pch.h"
#include "SkyDome.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"

// コンストラクタ
SkyDome::SkyDome()
{
	using namespace DirectX::SimpleMath;
	SetWorld(Matrix::Identity);
	AddComponent<ModelDraw>();
}

// デストラクタ
SkyDome::~SkyDome()
{
}

// 初期化処理
void SkyDome::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::SkyDome), true);
	GetComponent<ModelDraw>()->SetEmissiveColor({0.25f,0.25f,0.25f,1.0f});
}

// 更新処理
void SkyDome::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
}
