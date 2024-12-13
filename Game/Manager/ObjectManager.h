#pragma once

class GameObject;

class ObjectManager
{
public:
	static void Add(GameObject* object);
	static void Update(float elapsedTime);
	static void Remove(GameObject* object);
	static void Delete();
private:
	static std::vector<GameObject*> s_gameObjects;
	static std::vector<GameObject*> s_deleteObjects;
};
