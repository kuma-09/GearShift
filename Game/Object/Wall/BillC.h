#pragma once
#include "Game/Object/Wall/Wall.h"

/// <summary>
/// �r��C�N���X
/// </summary>
class BillC :public Wall
{
public:
	// �R���X�g���N�^
	BillC(IScene* scene);
	// �f�X�g���N�^
	~BillC();
	// ����������
	void Initialize() ;
	// �X�V����
	void Update(float elapsedtime);
};
