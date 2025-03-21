#pragma once
#include "Game/GameObject.h"

/// <summary>
/// �e�N���X�̊��N���X
/// </summary>
class Bullet :public GameObject
{
public:

	// �e�̏��
	enum BulletState
	{
		UNUSED,
		FLYING,
		USED,
	};

	// �R���X�g���N�^
	virtual ~Bullet() = default;
	// ����������
	virtual void Initialize(GameObject* object) = 0;
	// �e�𔭎�
	virtual void Shot(GameObject* target) = 0;
	// �e�������ɓ����������̏���
	virtual void Hit() = 0;
	// �X�V����
	virtual void Update(float elapsedTime) = 0;
	// �`�揈��
	virtual void Render() = 0;

	// �Q�b�^�[�Z�b�^�[
	//  �e�̏��
	void SetState(BulletState state) { m_state = state; }
	BulletState GetState() { return m_state; }

	// �^�[�Q�b�g
	void SetTarget(GameObject* target) { m_target = target; }
	GameObject* GetTarget() { return m_target; }

	// �e�𔭎˂����e
	GameObject* GetOwner() { return m_owner; }
	void SetOwner(GameObject* owner) { m_owner = owner; }

	// �U�����̒l
	void SetAttackPoint(int attackPoint) { m_attackPoint = attackPoint; }
	int GetAttackPoint() { return m_attackPoint; }

	// �����蔻��̏���
	virtual void Collision(Collider* collider) = 0;

private:
	// �e�̏��
	BulletState m_state;
	// �e���_���^�[�Q�b�g
	GameObject* m_target;
	// �e�𔭎˂����e
	GameObject* m_owner;
	// �U����
	int m_attackPoint;
};