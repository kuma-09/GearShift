#pragma once
#include "Game/Object/Wall/Wall.h"

/// <summary>
/// �r��B�N���X
/// </summary>
class BillB :public Wall
{
public:
	// �R���X�g���N�^
	BillB(IScene* scene);
	// �f�X�g���N�^
	~BillB();
	// ����������
	void Initialize() ;
	// �X�V����
	void Update(float elapsedtime);
};
