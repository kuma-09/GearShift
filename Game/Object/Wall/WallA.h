#pragma once
#include "Game/Object/Wall/Wall.h"

/// <summary>
/// ��A�N���X
/// </summary>
class WallA :public Wall
{
public:
	// �R���X�g���N�^
	WallA(Scene* scene);
	// �f�X�g���N�^
	~WallA();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedtime);
};
