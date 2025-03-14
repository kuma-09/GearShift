#pragma once
#include "Enemy.h"

class State;
class Bullet;
class EnemyAttackState;
class EnemyMoveState;

/// <summary>
/// �󒆂ɕ����G�l�~�[
/// </summary>
class HomingEnemy : public Enemy
{
public:
	// �R���X�g���N�^
	HomingEnemy(IScene* scene, GameObject* target);
	// �f�X�g���N�^
	~HomingEnemy();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �I������
	void Finalize();

	// �e�𔭎�
	void Shot();
	// �X�e�[�g��ύX
	void ChangeState(State* state);
	// �����蔻��̏���
	void Collision(Collider* collider);
private:
	// HP���c���Ă��邩�`�F�b�N
	void CheckHP();

private:
	// �e�̍ő吔
	const int BULLET_COUNT = 10;
	// �e�z��
	std::vector<std::unique_ptr<Bullet>> m_bullet;
	// �X�e�[�g
	State* m_state;
};

