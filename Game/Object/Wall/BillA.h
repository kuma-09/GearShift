#pragma once
#include "Game/Object/Wall/Wall.h"

/// <summary>
/// �r��A�N���X
/// </summary>
class BillA :public Wall
{
public:
	// �R���X�g���N�^
	BillA(Scene* scene);
	// �f�X�g���N�^
	~BillA();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedtime);
};
