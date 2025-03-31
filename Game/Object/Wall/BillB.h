#pragma once
#include "Game/Object/Wall/Wall.h"

/// <summary>
/// ビルBクラス
/// </summary>
class BillB :public Wall
{
public:
	// コンストラクタ
	BillB(IScene* scene);
	// デストラクタ
	~BillB();
	// 初期化処理
	void Initialize() ;
	// 更新処理
	void Update(float elapsedtime);
};
