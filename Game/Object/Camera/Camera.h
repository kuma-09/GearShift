#pragma once

#include "Game/GameObject.h"

class Graphics;
class InputManager;

/// <summary>
/// �J�����I�u�W�F�N�g
/// </summary>
class Camera :public GameObject
{
public:
	// �R���X�g���N�^
	Camera();
	// �f�X�g���N�^
	~Camera();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �^�[�Q�b�g���Z�b�g
	void SetTarget(GameObject* player, GameObject* target);
	// �J������h�炷
	void shake();
	// �J�����̏��
	enum CameraState
	{
		Follow,
		Girth,
	};

private:
	Graphics* m_graphics;
	InputManager* m_inputManager;
	GameObject* m_target;
	GameObject* m_player;

	DirectX::SimpleMath::Vector3 m_targetPosition;

	float m_rotateX;
	float m_rotateY;


	// �J�����̍���
	const float CAMERA_HEIGHT = 2.5f;

	// �J�����̋���
	const float CAMERA_DISTANCE = 10.f;

	// �J�����̒Ǐ]�W��
	const float CAMERA_EYE_RATE = 0.5f;

	// �^�[�Q�b�g�ɑ΂��Ă̌W��
	const float CAMERA_TARGET_RATE = 0.1f;

	// ��񓖂���̉�ʗh��̎���
	const float SHAKE_TIME = 0.1f;

	// ��ʗh��W��
	const float SHAKE_RATE = 1.0f;

	// ��ʗh��W��
	float m_shakeRate;

	// ��ʗh��o�ߎ���
	float m_shakeTime;
};