#pragma once

class Collider;

/// <summary>
/// コライダーコンポーネントを管理するマネージャー
/// </summary>
class CollisionManager
{
public:
	// コライダーコンポーネントをマネージャーに追加
	static void Add(Collider* component);
	// 更新処理
	static void Update();
	// 描画処理
	static void Render();
	// コンポーネントを削除
	static void Remove(Collider* component);
private:
	// 当たり判定をするか距離とコライダーの種類から判定
	static bool DistanceAndTypeIDCheck(Collider* collider1, Collider* collider2);
private:
	// コンポーネントをまとめた配列
	static std::vector<Collider*> s_colliders;
};
