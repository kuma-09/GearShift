#pragma once

class Collider;
class CollisionMesh;
#include "Game/Object/Floor/CollisionMesh.h"

class CollisionManager
{
public:
	
	static void InitializeCollisionMesh(const wchar_t* path);

	static void Add(Collider* component);
	static void Update();
	static void Remove(Collider* component);
private:
	static std::vector<Collider*> s_colliders;
	static std::unique_ptr<CollisionMesh> s_collisionMesh;
};
