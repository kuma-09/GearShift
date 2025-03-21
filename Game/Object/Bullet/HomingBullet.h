#pragma once
#include "Bullet.h"

/// <summary>
/// �^�[�Q�b�g��ǔ�����e
/// </summary>
class HomingBullet :public Bullet
{
public:
	// �R���X�g���N�^
	HomingBullet(IScene* scene, Collider::TypeID id);
	// �f�X�g���N�^
	~HomingBullet();	// ����������
	void Initialize(GameObject* object);
	// �e�𔭎�
	void Shot(GameObject* target);
	// �e�������ɓ����������̏���
	void Hit();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �����蔻��̏���
	void Collision(Collider* collider);

private:

	// �i�ޑ��x
	const float SPEED = 2.0f;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_velocity;
	// �o�ߎ���
	float m_period;
};