#include "pch.h"
#include "BurnerVertical.h"
#include "Game/Components/Trail.h"

// コンストラクタ
BurnerVertical::BurnerVertical()
{
	AddComponent<Trail>();
}
// デストラクタ
BurnerVertical::~BurnerVertical()
{
}
// 初期化処理
void BurnerVertical::Initialize()
{
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10, DirectX::Colors::CornflowerBlue);
}
// 更新処理
void BurnerVertical::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedTime);
	GetComponent<Trail>()->SetPos(GetPosition() - Vector3::Transform(Vector3(0.5f,0, 0),GetQuaternion()), GetPosition() + Vector3::Transform(Vector3(0.5f,0, 0), GetQuaternion()));
}
