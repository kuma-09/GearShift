#pragma once
#include "Game/Object/Wall/Wall.h"
/// <summary>
/// �r��D�N���X
/// </summary>
class BillD :public Wall
{
public:
	// �R���X�g���N�^
	BillD(Scene* scene);
	// �f�X�g���N�^
	~BillD();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedtime);
};
