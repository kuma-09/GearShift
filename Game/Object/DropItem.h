#pragma once
#include "Game/GameObject.h"
/// <summary>
/// ドロップアイテムクラス
/// </summary>
class DropItem :public GameObject
{
public:
	// コンストラクタ
	DropItem(IScene* scene);
	// デストラクタ
	~DropItem();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 当たり判定の処理
	void Collision(Collider* collider);
private:
	// モデルのサイズ
	const float MODEL_SIZE = 0.1f;
	// プレイヤーのHPを回復する値
	const int   RECOVERY_HP_POINT = 5;
};
