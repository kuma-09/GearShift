#pragma once
#include "Game/GameObject.h"
#include "Framework/Resources.h"
/// <summary>
/// ���N���X
/// </summary>
class Floor : public GameObject
{
public:
	// �R���X�g���N�^
	Floor(Resources::ModelType modelType);
	~Floor();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
};