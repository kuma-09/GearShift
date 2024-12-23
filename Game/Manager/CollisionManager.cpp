#include "pch.h"
#include "CollisionManager.h"
#include "Framework/Graphics.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Physics.h"
#include "Game/GameObject.h"

std::vector<Collider*> CollisionManager::s_colliders;
std::unique_ptr<CollisionMesh> CollisionManager::s_collisionMesh;

void CollisionManager::InitializeCollisionMesh(const wchar_t* path)
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//s_collisionMesh = std::make_unique<CollisionMesh>();
	//s_collisionMesh->Initialize(device, context, path);
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
		for (auto& collider2 : s_colliders)
		{
			if (collider1->GetBoundingBox()->Intersects(*collider2->GetBoundingBox()))
			{
				collider1->GetOwner()->Collision(collider2);
				//Ray ray{ collider1->GetOwner()->GetPosition() ,DirectX::SimpleMath::Vector3::Down };
				//Vector3 hitPosition;
				//Vector3 normal;
				//int polNum;
				//s_collisionMesh->IntersectRay(ray, &hitPosition, &normal, &polNum);
				//if (collider1->GetOwner()->GetPosition().y - collider1->GetBoundingBox()->Extents.y < hitPosition.y)
				//{
				//	// ‰Ÿ‚µ–ß‚·
				//	collider1->GetOwner()->SetPosition(Vector3(collider1->GetOwner()->GetPosition().x, hitPosition.y + collider1->GetBoundingBox()->Extents.y, collider1->GetOwner()->GetPosition().z));
				//	collider1->GetOwner()->GetComponent<Physics>()->Reset();
				//}
			}
		}
	}
}

void CollisionManager::Remove(Collider* component)
{
	s_colliders.erase(std::remove(s_colliders.begin(), s_colliders.end(), component), s_colliders.end());
}

