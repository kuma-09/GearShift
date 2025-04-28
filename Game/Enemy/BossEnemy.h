#pragma once
#include "Enemy.h"
#include "Game/Parts/Part.h"

#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Bullet/EnemyBullet.h"

#include "Game/Enemy/BossState/BossMissileState.h"
#include "Game/Enemy/BossState/BossTackleState.h"

class IState;

/// <summary>
/// �{�X�G�l�~�[
/// </summary>
class BossEnemy : public Enemy
{
public:

	// �R���X�g���N�^
	BossEnemy(Scene* scene, GameObject* target);
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
	void ChangeState(IState* state);

	// �~�T�C�����˃X�e�[�g���擾
	IState* GetMissileState() { return m_missileState.get(); }
	// �ˌ��X�e�[�g���擾
	IState* GetTackleState() { return m_tackleState.get(); }
	// �����蔻��̏���
	void Collision(Collider* collider);

private:

	void CreateBullets();
	void CreateStates();
	void CheckHP();

	// �e�̍ő吔
	const int MAX_BULLET = 10;
	const int MAX_HOMING_BULLET = 10;

	// �e�̔z��
	std::vector<std::unique_ptr<EnemyBullet>>  m_bullets;
	std::vector<std::unique_ptr<HomingBullet>> m_homingBullets;

	// �X�e�[�g
	IState* m_state;
	std::unique_ptr<BossMissileState> m_missileState;
	std::unique_ptr<BossTackleState> m_tackleState;
};