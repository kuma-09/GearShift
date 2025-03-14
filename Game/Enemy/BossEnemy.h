#pragma once
#include "Enemy.h"
#include "Game/Parts/Part.h"

#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Bullet/EnemyBullet.h"

#include "Game/Enemy/BossState/BossMissileState.h"
#include "Game/Enemy/BossState/BossTackleState.h"

class State;

/// <summary>
/// �{�X�G�l�~�[
/// </summary>
class BossEnemy : public Enemy
{
public:

	// �R���X�g���N�^
	BossEnemy(IScene* scene, GameObject* target);
	// �f�X�g���N�^
	~BossEnemy();

	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �I������
	void Finalize();

	// �K�g�����O�𔭎�
	void Shot();
	// �~�T�C���𔭎�
	void ShotMissile();

	// �K�g�����O�������[�h
	void ReloadGatling();
	// �X�e�[�g��ύX
	void ChangeState(State* state);

	// �~�T�C�����˃X�e�[�g���擾
	State* GetMissileState() { return m_missileState.get(); }
	// �ˌ��X�e�[�g���擾
	State* GetTackleState() { return m_tackleState.get(); }
	// �����蔻��̏���
	void Collision(Collider* collider);

	// �p�[�c���Z�b�g
	void SetPart(const Part::TypeID& partType, std::unique_ptr<Part> part)
	{
		m_pPart[partType] = std::move(part);
		m_pPart[partType]->SetOwner(this);
		m_pPart[partType]->Initialize(10,GetScene());
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

	void CreateBullets();
	void CreateStates();
	void CheckHP();

	// �p�[�c���܂Ƃ߂čX�V
	void UpdateParts(float elapsedTime) {
		for (auto& pair : m_pPart)
		{
			pair.second->Update(elapsedTime);
		}
	}

	// �p�[�c�z��
	std::unordered_map<Part::TypeID, std::unique_ptr<Part>> m_pPart;

	// �e�̍ő吔
	const int MAX_BULLET = 10;
	const int MAX_HOMING_BULLET = 10;

	// �e�̔z��
	std::vector<std::unique_ptr<EnemyBullet>>  m_bullets;
	std::vector<std::unique_ptr<HomingBullet>> m_homingBullets;

	// �X�e�[�g
	State* m_state;
	std::unique_ptr<BossMissileState> m_missileState;
	std::unique_ptr<BossTackleState> m_tackleState;
};