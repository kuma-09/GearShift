#pragma once
#include "Framework/Graphics.h"
#include "Component.h"

/// <summary>
/// �g���C���𐶐�����R���|�[�l���g�N���X
/// </summary>
class Trail : public Component
{
public:
	// �R���X�g���N�^
	Trail();
	// �f�X�g���N�^
	~Trail();
	// ����������
	void Initialize(const wchar_t* path,int bufferSize, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �g���C���𐶐�������W��ۑ�
	void SetPos(DirectX::XMFLOAT3 head, DirectX::XMFLOAT3 tail);
	// �ۑ����Ă�����W�z����N���A
	void ClearBuffer();
private:

	// �`��p�̃v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// �g���C���̍��W��ۑ�����\����
	struct PosBuffer
	{
		DirectX::XMFLOAT3 head; //��[�̈ʒu
		DirectX::XMFLOAT3 tail; //���[�̈ʒu
	};

	// 3D�ϊ��Ŏg���s��ƃg���C���̃J���[
	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMVECTOR color;
	};

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	// ���W��ۑ�����z��
	std::vector<PosBuffer> m_buffer;
	//	���̓��C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// �g���C���̃J���[
	DirectX::XMVECTORF32 m_color;
	// �ۑ��ł�����W�̐�
	int m_maxBufferSize;
	// ���ݕۑ����Ă�����W�̐�
	int m_bufferCount;
};