#include "pch.h"
#include "CollisionManager.h"
#include "Framework/Graphics.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Physics.h"
#include "Game/GameObject.h"

std::vector<Collider*> CollisionManager::s_colliders;

void CollisionManager::InitializeCollisionMesh(const wchar_t* path)
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
}

void CollisionManager::Add(Collider* collider)
{
	s_colliders.emplace_back(collider);
}

void CollisionManager::Update()
{
	using namespace DirectX::SimpleMath;

	for (auto& collider1 : s_colliders)
	{
		if (!collider1->GetActive()) continue;
		for (auto& collider2 : s_colliders)
		{
			if (!collider2->GetActive()) continue;
			if (collider1 == collider2) continue;
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

