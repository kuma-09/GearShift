#include "pch.h"
#include "SkyDome.h"
#include "Framework/Resources.h"

SkyDome::SkyDome()
{
	m_graphics = Graphics::GetInstance();
	SetWorld(DirectX::SimpleMath::Matrix::Identity);
}

SkyDome::~SkyDome()
{

}

void SkyDome::Update(float elapsedTime)
{
	using namespace DirectX;

	ComponentsUpdate(elapsedTime);
	// ���f���̃G�t�F�N�g�����X�V����
	Resources::GetInstance()->GetSkyDome()->UpdateEffects([](DirectX::IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				//// ���C�g��L��������
				//basicEffect->SetLightingEnabled(true);
				/*
					�E���f���N���X�ł̓f�t�H���g�œ����I�Ƀ��C�g���L��������Ă���
					�E���̐ݒ肪�����Ă����C�g�͗L��������Ă���
				*/

				// �ʂ̃��C�g�����ׂĖ���������
				basicEffect->SetLightEnabled(0, false);
				basicEffect->SetLightEnabled(1, false);
				basicEffect->SetLightEnabled(2, false);

				// ���f����������������
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
}

void SkyDome::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	
	Resources::GetInstance()->GetSkyDome()->Draw(context, *state, GetWorld(), view, projection, false, [&]()
		{
			context->RSSetState(state->CullNone());
		}
	);
}