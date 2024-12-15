#include "pch.h"
#include "CollisionManager.h"
#include "Game/Components/Collider.h"
#include "Game/GameObject.h"

std::vector<Collider*> CollisionManager::s_colliders;

void CollisionManager::Add(Collider* collider)
{
	s_colliders.emplace_back(collider);
}

void CollisionManager::Update()
{
	for (auto& collider1 : s_colliders)
	{
		for (auto& collider2 : s_colliders)
		{
			if (collider1->GetBoundingBox()->Intersects(*collider2->GetBoundingBox()))
			{
				collider1->GetOwner()->Collision(collider2);
			}
		}
	}
}

void CollisionManager::Remove(Collider* component)
{
	s_colliders.erase(std::remove(s_colliders.begin(), s_colliders.end(), component), s_colliders.end());
}

