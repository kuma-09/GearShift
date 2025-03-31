#include "pch.h"
#include "Ceiling.h"
#include "Game/Components/Collider.h"
// コンストラクタ
Ceiling::Ceiling()
{
	AddComponent<Collider>();
}
// デストラクタ
Ceiling::~Ceiling()
{
}
// 初期化処理
void Ceiling::Initialize()
{
	SetScale({ 300, 1.25f, 300 });
	GetComponent<Collider>()->Initialize(Collider::Floor, Collider::Fixed,GetScale(),{0,-1,0});
}
// 更新処理
void Ceiling::Update(float elapsedtime)
{
	ComponentsUpdate(elapsedtime);
}
// 描画処理
void Ceiling::Render()
{
}
