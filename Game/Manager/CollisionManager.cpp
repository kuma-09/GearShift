#include "pch.h"
#include "CollisionManager.h"
#include "Framework/Graphics.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Physics.h"
#include "Game/GameObject.h"

std::vector<Collider*> CollisionManager::s_colliders;

// �R���C�_�[�R���|�[�l���g���}�l�[�W���[�ɒǉ�
void CollisionManager::Add(Collider* collider)
{
	s_colliders.emplace_back(collider);
}

// �X�V����
void CollisionManager::Update()
{
	using namespace DirectX::SimpleMath;

	for (auto& collider1 : s_colliders)
	{
		// �A�N�e�B�u����Ȃ��I�u�W�F�N�g�̓X�L�b�v
		if (!collider1->GetActive()) continue;
		for (auto& collider2 : s_colliders)
		{
			// �A�N�e�B�u����Ȃ��I�u�W�F�N�g�̓X�L�b�v
			if (!collider2->GetActive()) continue;

			// �����蔻����v�Z���鋗�����ނ�����
			if (DistanceAndTypeIDCheck(collider1,collider2))
			{
				// �����蔻����v�Z
				if (collider1->GetBoundingBox()->Intersects(*collider2->GetBoundingBox()))
				{
					collider1->GetOwner()->Collision(collider2);
				}
			}
		}
	}
}

// �`�揈��
void CollisionManager::Render()
{
	for (auto& collider : s_colliders)
	{
		collider->Render();
	}
}

// �R���|�[�l���g���폜
void CollisionManager::Remove(Collider* component)
{
	s_colliders.erase(std::remove(s_colliders.begin(), s_colliders.end(), component), s_colliders.end());
}

// �����蔻������邩�����ƃR���C�_�[�̎�ނ��画��
bool CollisionManager::DistanceAndTypeIDCheck(Collider* collider1, Collider* collider2)
{
	using namespace DirectX::SimpleMath;

	if (collider1 == collider2) return false;
	auto collider1BoudingBox = collider1->GetBoundingBox();
	auto collider2BoudingBox = collider2->GetBoundingBox();

	// �����̃I�u�W�F�N�g�̓X���[
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

