#pragma once
#include "Game/GameObject.h"

/// <summary>
/// 天井クラス
/// </summary>
class Ceiling : public GameObject
{
public:
	// コンストラクタ
	Ceiling();
	// デストラクタ
	~Ceiling();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedtime);
	// 描画処理
	void Render();
private:

};