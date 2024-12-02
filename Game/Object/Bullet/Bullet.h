#pragma once
#include "Game/GameObject.h"

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


	virtual ~Bullet() = default;
	virtual void Initialize(GameObject* object) = 0;
	virtual void Shot(GameObject* target) = 0;
	virtual void Hit() = 0;
	virtual void Update(float elapsedTime) = 0;
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

	virtual void Collision(Collider* collider) = 0;

private:
	BulletState m_state;
	GameObject* m_target;
	GameObject* m_owner;
};