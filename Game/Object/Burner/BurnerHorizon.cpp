#include "pch.h"
#include "BurnerHorizon.h"
#include "Game/Components/Trail.h"

// コンストラクタ
BurnerHorizon::BurnerHorizon()
{
	AddComponent<Trail>();
}
// デストラクタ
BurnerHorizon::~BurnerHorizon()
{
}
// 初期化処理
void BurnerHorizon::Initialize()
{
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10, DirectX::Colors::CornflowerBlue);
}
// 更新処理
void BurnerHorizon::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	GetComponent<Trail>()->SetPos(GetPosition() - Vector3(0,0.5f, 0), GetPosition() + Vector3(0,0.5f, 0));
}
