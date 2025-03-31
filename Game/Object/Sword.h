#pragma once
#include "Game/GameObject.h"

/// <summary>
/// �v���C���[�U���p�̃u���[�h�N���X
/// </summary>
class Blade :public GameObject
{
public:

	// �u���[�h�̏��
	enum BladeState
	{
		UNUSED,
		USING,
		USED,
	};

	// �R���X�g���N�^
	Blade(IScene* scene, Collider::TypeID id);
	// �f�X�g���N�^
	~Blade();
	// ����������
	void Initalize(GameObject* object);
	// �e�𔭎�
	void Shot(GameObject* object);
	// �����ɓ����������̏���
	void Hit();
	// �X�V����
	void Update(float elapsedTime);

	// �u���[�h�̏�Ԃ��擾
	bool GetIsHit() { return m_isHit; }
	BladeState GetState() { return m_state; }
	void SetState(BladeState state) { m_state = state; }

	// �����蔻��̏���
	void Collision(Collider* collider);

private:
	// �X�e�[�g
	BladeState m_state;
	// �e
	GameObject* m_owner;

	bool m_isHit;
	float m_rotate;
	const DirectX::SimpleMath::Vector3 SIZE = {5.f,5.f,5.f};
};