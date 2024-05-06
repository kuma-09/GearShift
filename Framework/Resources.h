#pragma once
#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

#include "Model.h"
#include "Graphics.h"

// Resources�N���X���`����
class Resources
{
public:
	// ���f�����擾����
	DirectX::Model* GetModel() { return m_model.get(); }
	// �e�N�X�`�����擾
	ID3D11ShaderResourceView* GetTexture() { return m_texture.Get(); }
public:
	Resources(Resources&&) = default;
	Resources& operator= (Resources&&) = default;
	Resources& operator= (Resources const&) = delete;
	// �f�X�g���N�^
	~Resources() = default;
	// Resouces�N���X�̃C���X�^���X���擾����
	static Resources* const GetInstance();

	void Initalize(Graphics* graphics);

	// ���\�[�X�����[�h����
	void LoadResource();

private:
	// �R���X�g���N�^
	Resources() noexcept
		:
		m_graphics{},										// �O���t�B�b�N�X
		m_device{},											// �f�o�C�X
		m_model{},											// ���f��
		m_texture{}											// �e�N�X�`��

	{
	}

private:
	// ���\�[�X
	static std::unique_ptr<Resources> m_resources;
	// �O���t�B�b�N�X
	Graphics* m_graphics;
	// �f�o�C�X
	ID3D11Device* m_device;

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

#endif		// RESOURCES_DEFINED
