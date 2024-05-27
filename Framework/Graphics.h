#pragma once
#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED

#include "DeviceResources.h"

class Graphics final
{
public:
	// �f�o�C�X���\�[�X���擾����
	DX::DeviceResources* GetDeviceResources() { return m_deviceResources.get(); }
	// �X�N���[���T�C�Y���擾����
	void GetScreenSize(int& width, int& height) const { width = m_screenW; height = m_screenH; }
	// �X�N���[���T�C�Y��ݒ肷��
	void SetScreenSize(const int& width, const int& height) { m_screenW = width; m_screenH = height; }
	// PrimitiveBatch�N���X�̃C���X�^���X���擾����
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* GetPrimitiveBatch() const { return m_primitiveBatch.get(); }
	// PrimitiveBatch�N���X�̃C���X�^���X���擾����
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetPrimitiveBatchPositionColor() const { return m_primitiveBatchPositionColor.get(); }
	// InputLayout�N���X�̃C���X�^���X���擾����
	ID3D11InputLayout* GetInputLayout() const { return m_inputLayout.Get(); }
	// BasicEffect�N���X�̃C���X�^���X�擾����
	DirectX::BasicEffect* GetBasicEffect() { return m_basicEffect.get(); }
	// CommonStates�N���X�̃C���X�^���X���擾����
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates.get(); }
	// SpriteBatch�N���X�̃C���X�^���X���擾����
	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	// SpriteFont�N���X�̃C���X�^���X���擾����
	DirectX::SpriteFont* GetFont() { return m_spriteFont.get(); }
	// EffectFactory�N���X�̃C���X�^���X���擾����
	DirectX::EffectFactory* GetFX() const { return m_effectFactory.get(); }
	// �r���[�s���ݒ肷��
	void SetViewMatrix(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
	// �r���[�s����擾����
	const DirectX::SimpleMath::Matrix& GetViewMatrix() { return m_view; };
	// �ˉe�s���ݒ肷��
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }
	// �ˉe�s����擾����
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() { return m_projection; };

private:
	// �R���X�g���N�^
	Graphics();
	// �C���X�^���X���R�s�[���邱�Ƃ��֎~����
	void operator=(const Graphics&) = delete;
	// �C���X�^���X�����[�u���邱�Ƃ��֎~����
	Graphics& operator= (Graphics&&) = delete;
	// �R�s�[�R���X�g���N�^�͋֎~����
	Graphics(const Graphics&) = delete;
	// ���[�u�R���X�g���N�^�͋֎~����
	Graphics(Graphics&&) = delete;

public:
	static Graphics* GetInstance();

	// ����������
	void Initialize();
	// �f�X�g���N�^
	~Graphics() = default;

	// �v���~�e�B�u�`����J�n����
	void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, const DirectX::SimpleMath::Matrix& world);
	void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector4 color);
	// �v���~�e�B�u�`����I������
	void DrawPrimitiveEnd();

	// �v���~�e�B�u�`����J�n����
	void DrawPrimitivePositionColorBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// �v���~�e�B�u�`����I������
	void DrawPrimitivePositionColorEnd();

private:
	// �O���t�B�b�N�X
	static std::unique_ptr<Graphics> m_graphics;
	// �f�o�C�X���\�[�X
	std::unique_ptr<DX::DeviceResources> m_deviceResources;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonStates;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatchPositionColor;
	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_effectFactory;
	// ���X�^���C�U�[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterrizerState;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �X�N���[���T�C�Y
	int m_screenW, m_screenH;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �f�o�C�X
	ID3D11Device* m_device;
	// �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_context;
};
#endif		// GRAPHICS_DEFINED

