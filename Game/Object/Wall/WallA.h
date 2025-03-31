#pragma once
#include "Game/Object/Wall/Wall.h"

/// <summary>
/// 壁Aクラス
/// </summary>
class WallA :public Wall
{
public:
	// コンストラクタ
	WallA(IScene* scene);
	// デストラクタ
	~WallA();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedtime);
};
