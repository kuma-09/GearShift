#pragma once
#include "Bullet.h"

class Player;

/// <summary>
/// �G�����ʏ�e(�\���ˌ�)
/// </summary>
class EnemyBullet :public Bullet
{
public:
	// �R���X�g���N�^
	EnemyBullet(Scene* scene, Collider::TypeID id);
	// �f�X�g���N�^
	~EnemyBullet();
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
	DirectX::SimpleMath::Vector3 LinePrediction(Player* target);
	float PlusMin(float a, float b);

private:

	const float SPEED = 1.5f;
	const float DIFFUSION = 1.0f;

};