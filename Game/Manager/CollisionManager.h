#pragma once

class Collider;

class CollisionManager
{
public:
	
	static void InitializeCollisionMesh(const wchar_t* path);

	static void Add(Collider* component);
	static void Update();
	static void Render();
	static void Remove(Collider* component);
private:
	static std::vector<Collider*> s_colliders;
};
