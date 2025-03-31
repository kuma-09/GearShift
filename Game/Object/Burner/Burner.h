#pragma once

#include "BurnerVertical.h"
#include "BurnerHorizon.h"

/// <summary>
/// �c�������̃g���C�����܂Ƃ߂��o�[�i�[�N���X
/// </summary>
class Burner
{
public:
	// �R���X�g���N�^
	Burner();
	// �f�X�g���N�^
	~Burner();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime ,DirectX::SimpleMath::Vector3 pos,DirectX::SimpleMath::Quaternion rotate);
	// �g���C�����N���A
	void ClearTrail();
	// �e���擾
	GameObject* GetOwner() { return m_owner; }
private:
	// �e
	GameObject* m_owner;
	// �������̃g���C��
	std::unique_ptr<BurnerVertical> m_vertical;
	// �c�����̃g���C��
	std::unique_ptr<BurnerHorizon>  m_horizon;
};