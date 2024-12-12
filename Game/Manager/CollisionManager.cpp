#include "pch.h"
#include "CollisionManager.h"
#include "Game/Components/Collider.h"

std::vector<Collider*> CollisionManager::s_colliders;

void CollisionManager::Add(Collider* collider)
{
	s_colliders.emplace_back(collider);
}

void CollisionManager::Clear()
{
	s_colliders.clear();
}
