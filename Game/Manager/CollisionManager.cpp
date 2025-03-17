#include "pch.h"
#include "CollisionManager.h"
#include "Framework/Graphics.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Physics.h"
#include "Game/GameObject.h"

std::vector<Collider*> CollisionManager::s_colliders;

// コライダーコンポーネントをマネージャーに追加
void CollisionManager::Add(Collider* collider)
{
	s_colliders.emplace_back(collider);
}

// 更新処理
void CollisionManager::Update()
{
	using namespace DirectX::SimpleMath;

	for (auto& collider1 : s_colliders)
	{
		// アクティブじゃないオブジェクトはスキップ
		if (!collider1->GetActive()) continue;
		for (auto& collider2 : s_colliders)
		{
			// アクティブじゃないオブジェクトはスキップ
			if (!collider2->GetActive()) continue;

			// 当たり判定を計算する距離や種類か判別
			if (DistanceAndTypeIDCheck(collider1,collider2))
			{
				// 当たり判定を計算
				if (collider1->GetBoundingBox()->Intersects(*collider2->GetBoundingBox()))
				{
					collider1->GetOwner()->Collision(collider2);
				}
			}
		}
	}
}

// 描画処理
void CollisionManager::Render()
{
	for (auto& collider : s_colliders)
	{
		collider->Render();
	}
}

// コンポーネントを削除
void CollisionManager::Remove(Collider* component)
{
	s_colliders.erase(std::remove(s_colliders.begin(), s_colliders.end(), component), s_colliders.end());
}

// 当たり判定をするか距離とコライダーの種類から判定
bool CollisionManager::DistanceAndTypeIDCheck(Collider* collider1, Collider* collider2)
{
	using namespace DirectX::SimpleMath;

	if (collider1 == collider2) return false;
	auto collider1BoudingBox = collider1->GetBoundingBox();
	auto collider2BoudingBox = collider2->GetBoundingBox();

	// 遠くのオブジェクトはスルー
	if (collider2->GetColliderType() == Collider::Fixed)
	{
		if ((collider1BoudingBox->Center - collider2BoudingBox->Center).Length() >= 100 && collider2->GetTypeID() != Collider::Floor) false;
	}
	else
	{
		if ((collider1BoudingBox->Center - collider2BoudingBox->Center).Length() >= 10) false;
	}

	return true;
}

