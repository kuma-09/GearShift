#include "pch.h"
#include "SkyDome.h"
#include "Framework/Resources.h"

SkyDome::SkyDome()
{
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	SetWorld(SimpleMath::Matrix::Identity);
	// モデルのエフェクト情報を更新する
	Resources::GetInstance()->GetSkyDome()->UpdateEffects([](IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// 個別のライトをすべて無効化する
				basicEffect->SetLightEnabled(0, false);
				basicEffect->SetLightEnabled(1, false);
				basicEffect->SetLightEnabled(2, false);

				// モデルを自発光させる
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
}

SkyDome::~SkyDome()
{

}

void SkyDome::Initialize(DirectX::SimpleMath::Vector3 pos)
{
	SetWorld(DirectX::SimpleMath::Matrix::CreateTranslation(pos));
}

void SkyDome::Update(float elapsedTime)
{

	ComponentsUpdate(elapsedTime);

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