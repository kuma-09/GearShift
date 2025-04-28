#pragma once
#include "Bullet.h"

class Player;

/// <summary>
/// �G�����ʏ�e
/// </summary>
class FixedEnemyBullet :public Bullet
{
public:

	// �R���X�g���N�^
	FixedEnemyBullet(Scene* scene, Collider::TypeID id);
	// �f�X�g���N�^
	~FixedEnemyBullet();
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
	// �i�ޑ��x�@
	const float SPEED = 2.5f;
};