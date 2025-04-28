#pragma once
#include "Component.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

/// <summary>
/// ���f���`��R���|�[�l���g�N���X
/// </summary>
class ModelDraw : public Component
{
public:
	// �R���X�g���N�^
	ModelDraw();
	// �f�X�g���N�^
	~ModelDraw();

	// ����������
	void Initialize(DirectX::Model* model,bool texture = false);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �V���h�E�}�b�v���쐬
	void CreateShadow();

	// �������C�g�J���[��ݒ�
	void SetRimLithgColor(DirectX::XMVECTORF32 color);
	// �G�~�b�V�u�J���[��ݒ�
	void SetEmissiveColor(DirectX::XMVECTORF32 color);

private:
	Graphics* m_graphics;

	// �`�悷�郂�f��
	DirectX::Model* m_model;
	// �`�悷�郏�[���h���W
	DirectX::SimpleMath::Matrix m_world;
	// �e�N�X�`�������郂�f����
	bool m_isTexture;
	// �������C�g��K�p���邩
	bool m_isRimLight;
	// �������C�g�J���[
	DirectX::XMVECTORF32 m_rimColor;
	// �G�~�b�V�u�J���[
	DirectX::XMVECTORF32 m_emissiveColor;
};

