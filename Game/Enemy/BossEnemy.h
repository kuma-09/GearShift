#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Enemy.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Parts/Part.h"

class State;
class FixedEnemyBullet;
class EnemyAttackState;
class EnemyMoveState;

class BossEnemy : public Enemy
{
public:

	BossEnemy(IScene* scene);
	~BossEnemy();

	void Initialize(GameObject* target);
	void Update(float elapsedTime);
	void CreateShader();
	void Render();
	void Finalize();


	void Shot();
	void ChangeState(State* state);

	void Collision(BoxCollider* collider);

	// �p�[�c���Z�b�g
	void SetPart(const Part::TypeID& partType, std::unique_ptr<Part> part)
	{
		m_pPart[partType] = std::move(part);
		m_pPart[partType]->SetOwner(this);
		m_pPart[partType]->Initialize(10, GetScene());
	}

	// �p�[�c���擾
	Part* GetPart(const Part::TypeID& partType)
	{
		if (m_pPart.find(partType) != m_pPart.end())
		{
			return m_pPart[partType].get();
		}
		return nullptr;
	}

private:

	// �p�[�c���܂Ƃ߂čX�V
	void UpdateParts(float elapsedTime) {
		for (auto& pair : m_pPart)
		{
			pair.second->Update(elapsedTime);
		}
	}

	// �p�[�c���܂Ƃ߂ĕ`��
	void RenderParts() {
		for (auto& pair : m_pPart)
		{
			pair.second->Render();
		}
	}

	void CreateShadows()
	{
		for (auto& pair : m_pPart)
		{
			pair.second->CreateShadow();
		}
	}

	// �p�[�c�z��
	std::unordered_map<Part::TypeID, std::unique_ptr<Part>> m_pPart;

	std::unique_ptr<Bullet> m_bullet;

	State* m_state;
};

