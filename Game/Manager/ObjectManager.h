#pragma once

#include "Game/GameObject.h"

/// <summary>
/// ゲームオブジェクトを管理するマネージャー
/// </summary>
class ObjectManager
{
public:
	// ゲームオブジェクトをマネージャーに追加する
	static std::weak_ptr<GameObject> Add(std::shared_ptr<GameObject> object,DirectX::SimpleMath::Vector3 pos,ObjectType::TypeID type = ObjectType::None);
	// 更新処理
	static void Update(float elapsedTime);
	// ゲームオブジェクトを削除予定配列に追加
	static void Remove(GameObject* object);
	// 削除予定オブジェクトをまとめて削除
	static void Delete();
	// すべてのゲームオブジェクトを削除
	static void Clear();
	// タイプごとにオブジェクトを取得
	static std::vector<std::weak_ptr<GameObject>> GetTypeObjects(ObjectType::TypeID type);
private:
	// ゲームオブジェクトの配列
	static std::vector<std::shared_ptr<GameObject>> s_gameObjects;
	// 削除予定のゲームオブジェクト
	static std::vector<GameObject*> s_deleteObjects;
};
