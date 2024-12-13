#include "pch.h"	
#include "ObjectManager.h"
#include "Game/GameObject.h"

std::vector<GameObject*> ObjectManager::s_gameObjects;
std::vector<GameObject*> ObjectManager::s_deleteObjects;

void ObjectManager::Add(GameObject* object)
{
	s_gameObjects.emplace_back(object);
}

void ObjectManager::Update(float elapsedTime)
{
	for (auto& object : s_gameObjects)
	{
		object->Update(elapsedTime);
	}
}

void ObjectManager::Remove(GameObject* object)
{
	s_deleteObjects.emplace_back(object);
}

void ObjectManager::Delete()
{
	for (auto& object: s_deleteObjects)
	{
		s_gameObjects.erase(std::remove(s_gameObjects.begin(), s_gameObjects.end(), object), s_gameObjects.end());
	}
	s_deleteObjects.clear();
}
