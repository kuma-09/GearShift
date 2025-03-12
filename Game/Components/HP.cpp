#include "pch.h"
#include "HP.h"

// コンストラクタ
HP::HP()
	:m_hp{}
{
}

// デストラクタ
HP::~HP()
{

}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="hp">初期HP</param>
void HP::Initialize(int hp)
{
	SetHP(hp);
}

void HP::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}
