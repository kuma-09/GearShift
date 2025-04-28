#pragma once
#include "Enemy.h"

class IState;
class FixedEnemyBullet;

/// <summary>
/// �Œ�G�l�~�[
/// </summary>
class FixedEnemy : public Enemy
{
public:
	// �R���X�g���N�^
	FixedEnemy(Scene* scene, GameObject* target);
	// �f�X�g���N�^
	~FixedEnemy();
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
	void ChangeState(IState* state);
	// �����蔻��̏���
	void Collision(Collider* collider);

private:
	void CheckHP();
private:
	// �e�z��
	std::unique_ptr<FixedEnemyBullet> m_bullet;
	// �X�e�[�g
	IState* m_state;
};

