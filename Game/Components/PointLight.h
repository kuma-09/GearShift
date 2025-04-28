#pragma once
#include "Component.h"

/// <summary>
/// �|�C���g���C�g�R���|�[�l���g�N���X
/// </summary>
class PointLight : public Component
{
public:
	// �R���X�g���N�^
	PointLight();
	// �f�X�g���N�^
	~PointLight();
	// ����������
	void Initialize(DirectX::SimpleMath::Vector3 = {1,1,1});
	// �X�V����
	void Update(float elapsedTime);
	// �J���[�����Z�b�g
	void ClearColor();
	// �|�C���g���C�g�̐F���擾
	DirectX::SimpleMath::Vector3 GetColor() { return m_nowColor; }

private:
	// �J���[
	DirectX::SimpleMath::Vector3 m_targetColor;
	DirectX::SimpleMath::Vector3 m_nowColor;
	// �o�ߎ���
	float m_nowTime;
};

