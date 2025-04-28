#pragma once
#include "Bullet.h"

class Player;

/// <summary>
/// �v���C���[���g�p����ʏ�e
/// </summary>
class NormalBullet :public Bullet
{
public:
	// �R���X�g���N�^
	NormalBullet(Scene* scene, Collider::TypeID id);
	// �f�X�g���N�^
	~NormalBullet();
	// ����������
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

	// ��������
	const float MAX_TIME = 10.0f;
	// �o�ߎ���
	float m_totalTime;
	// �i�ޑ��x
	const float SPEED = 100.0f;

};