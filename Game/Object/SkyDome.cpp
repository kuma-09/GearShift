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
	// モデルのエフェクト情報を更新する
	Resources::GetInstance()->GetSkyDome()->UpdateEffects([](DirectX::IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				//// ライトを有効化する
				//basicEffect->SetLightingEnabled(true);
				/*
					・モデルクラスではデフォルトで内部的にライトが有効化されている
					・この設定が無くてもライトは有効化されている
				*/

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