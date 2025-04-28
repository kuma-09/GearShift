#pragma once
#include "Game/Object/Wall/Wall.h"
/// <summary>
/// ビルDクラス
/// </summary>
class BillD :public Wall
{
public:
	// コンストラクタ
	BillD(Scene* scene);
	// デストラクタ
	~BillD();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedtime);
};
