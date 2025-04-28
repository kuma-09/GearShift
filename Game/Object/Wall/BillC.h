#pragma once
#include "Game/Object/Wall/Wall.h"

/// <summary>
/// ビルCクラス
/// </summary>
class BillC :public Wall
{
public:
	// コンストラクタ
	BillC(Scene* scene);
	// デストラクタ
	~BillC();
	// 初期化処理
	void Initialize() ;
	// 更新処理
	void Update(float elapsedtime);
};
