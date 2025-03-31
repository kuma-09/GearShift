#include "pch.h"	
#include "ObjectManager.h"

std::vector<std::shared_ptr<GameObject>> ObjectManager::s_gameObjects;
std::vector<GameObject*> ObjectManager::s_deleteObjects;

// ゲームオブジェクトをマネージャーに追加する
std::weak_ptr<GameObject> ObjectManager::Add(std::shared_ptr<GameObject> object, DirectX::SimpleMath::Vector3 pos, ObjectType::TypeID type)
{
	s_gameObjects.emplace_back(object);
	s_gameObjects.back()->Initialize();
	s_gameObjects.back()->SetType(type);
	s_gameObjects.back()->SetPosition(pos);
	s_gameObjects.back()->SetWorld(DirectX::SimpleMath::Matrix::CreateTranslation(pos));
	return std::static_pointer_cast<GameObject>(object);
}

// 更新処理
void ObjectManager::Update(float elapsedTime)
{
	for (auto& object : s_gameObjects)
	{
		object->Update(elapsedTime);
	}
}

// ゲームオブジェクトを削除予定配列に追加
void ObjectManager::Remove(GameObject* object)
{
	s_deleteObjects.emplace_back(object);
}

// 削除予定オブジェクトをまとめて削除
void ObjectManager::Delete()
{
	for (auto& deleteObject : s_deleteObjects)
	{
		// 削除対象を検索
		auto it = std::remove_if(
			s_gameObjects.begin(),
			s_gameObjects.end(),
			[deleteObject](const std::shared_ptr<GameObject>& obj) {
				return obj.get() == deleteObject; // 内部ポインタを比較
			});

		// 削除対象をリストから削除
		s_gameObjects.erase(it, s_gameObjects.end());
	}
	// 削除予定配列をクリア
	s_deleteObjects.clear();
}

// すべてのゲームオブジェクトを削除
void ObjectManager::Clear()
{
	s_gameObjects.clear();
}

// タイプごとにオブジェクトを取得
std::vector<std::weak_ptr<GameObject>> ObjectManager::GetTypeObjects(ObjectType::TypeID type)
{
	std::vector<std::weak_ptr<GameObject>> results;

	for (auto& object : s_gameObjects)
	{
		if (object->GetType() == type)
		{
			results.emplace_back(std::static_pointer_cast<GameObject>(object));
		}
	}
	return results;
}
