#pragma once
#include "Game/GameObject.h"

/// <summary>
/// �V��N���X
/// </summary>
class Ceiling : public GameObject
{
public:
	// �R���X�g���N�^
	Ceiling();
	// �f�X�g���N�^
	~Ceiling();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedtime);
	// �`�揈��
	void Render();
private:

};