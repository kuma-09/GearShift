#pragma once
#include "Component.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

/// <summary>
/// HP��`�悷��R���|�[�l���g�N���X
/// </summary>
class HPBar : public Component
{
public:
	// �R���X�g���N�^
	HPBar();
	// �f�X�g���N�^
	~HPBar();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��(2D���
	void Render(DirectX::SimpleMath::Vector2 position);
	// �`�揈��(3D���
	void Render(DirectX::SimpleMath::Vector3 position);

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// HP�̍ő�l
	int  m_maxHp;
	// �c���HP
	float m_greenHp;
	// ��������HP
	float m_redHp;
	// �o�ߎ���
	float m_nowTime;

	// HP�`��p�̎l���_
	DirectX::VertexPositionColorTexture m_vertex[4];
	DirectX::VertexPositionColorTexture m_redVertex[4];
	DirectX::VertexPositionColorTexture m_backVertex[4];

	// �e�N�X�`��
	ID3D11ShaderResourceView* m_texture;

};

