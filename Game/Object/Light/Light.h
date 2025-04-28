#pragma once

#include "Game/GameObject.h"

/// <summary>
/// ポイントライトオブジェクト
/// </summary>
class Light : public GameObject
{
public:
	// コンストラクタ
	Light(Scene* scene);
	// デストラクタ
	~Light();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedtime);
	// 描画処理
	void Render();

private:

};
