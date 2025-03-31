#pragma once
#include "Game/Object/Wall/Wall.h"

/// <summary>
/// ビルAクラス
/// </summary>
class BillA :public Wall
{
public:
	// コンストラクタ
	BillA(IScene* scene);
	// デストラクタ
	~BillA();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedtime);
};
