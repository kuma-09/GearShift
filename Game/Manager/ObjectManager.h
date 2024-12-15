#pragma once

#include "Game/GameObject.h"

class ObjectManager
{
public:
	static std::weak_ptr<GameObject> Add(std::shared_ptr<GameObject> object,DirectX::SimpleMath::Vector3 pos,Type::TypeID type = Type::None);
	static void Update(float elapsedTime);
	static void Remove(GameObject* object);
	static void Delete();
	static void Clear();

	static std::vector<std::weak_ptr<GameObject>> GetTypeObjects(Type::TypeID type);
private:
	static std::vector<std::shared_ptr<GameObject>> s_gameObjects;
	static std::vector<GameObject*> s_deleteObjects;
};
