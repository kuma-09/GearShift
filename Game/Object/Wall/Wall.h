#pragma once
#include "Game/GameObject.h"
/// <summary>
/// 壁やビルの基底クラス
/// </summary>
class Wall :public GameObject
{
public:
	// コンストラクタ
	Wall() {};
	// デストラクタ
	virtual ~Wall() = default;
	// 初期化処理
	virtual void Initialize() = 0;
	// 更新処理
	virtual void Update(float elapsedtime) = 0;
};
