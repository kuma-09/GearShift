#pragma once
#include "Game/GameObject.h"
#include "Framework/Graphics.h"

/// <summary>
/// �V���N���X
/// </summary>
class SkyDome :public GameObject
{
public:
	// �R���X�g���N�^
	SkyDome();
	// �f�X�g���N�^
	~SkyDome();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
private:
};
