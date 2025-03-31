#pragma once
#include "Game/GameObject.h"
#include "Framework/Resources.h"
/// <summary>
/// 床クラス
/// </summary>
class Floor : public GameObject
{
public:
	// コンストラクタ
	Floor(Resources::ModelType modelType);
	~Floor();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
};