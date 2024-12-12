#pragma once

class Collider;

class CollisionManager
{
public:
	static void Add(Collider* collider);
	static void Render();
	static void Clear();
private:
	static std::vector<Collider*> s_colliders;
};
