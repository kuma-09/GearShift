#pragma once
#include "Game/GameObject.h"
/// <summary>
/// �ǂ�r���̊��N���X
/// </summary>
class Wall :public GameObject
{
public:
	// �R���X�g���N�^
	Wall() {};
	// �f�X�g���N�^
	virtual ~Wall() = default;
	// ����������
	virtual void Initialize() = 0;
	// �X�V����
	virtual void Update(float elapsedtime) = 0;
};
