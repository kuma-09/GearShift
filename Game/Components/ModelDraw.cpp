#include "pch.h"
#include "ModelDraw.h"
#include "Game/GameObject.h"
#include "Game/Shader/Shader.h"
#include "Game/DeferredRendering.h"
#include "Game/Manager/RenderManager.h"
#include "Game/Shader/ShadowMap.h"

// コンストラクタ
ModelDraw::ModelDraw()
	:
	m_model{},
	m_isTexture{},
	m_isRimLight{},
	m_rimColor{},
	m_emissiveColor{}
{
	m_graphics = Graphics::GetInstance();
}

// デストラクタ
ModelDraw::~ModelDraw()
{
	RenderManager::Remove(this);
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="model">描画モデル</param>
/// <param name="texture">テクスチャの有無</param>
void ModelDraw::Initialize(DirectX::Model* model,bool texture)
{
	m_model = model;
	m_isTexture = texture;

	// レンダーマネージャーにモデル描画コンポーネントを追加
	RenderManager::Add(this);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime"></param>
void ModelDraw::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}


/// <summary>
/// 描画処理
/// </summary>
void ModelDraw::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	auto world = GetOwner()->GetWorld();

	m_model->Draw(context, *state, world, view, projection, false, [&]
	{
		// GBufferにモデルを描画
		DeferredRendering::DrawGBuffer(m_isTexture,m_isRimLight,m_rimColor,m_emissiveColor);
	});
}

/// <summary>
/// シャドウマップを作成
/// </summary>
void ModelDraw::CreateShadow()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	auto world = GetOwner()->GetWorld();

	m_model->Draw(context, *state, world, view, projection, false, [&]
	{
		ShadowMap::RenderDepth();
	});
}

/// <summary>
/// リムライトカラーを設定
/// </summary>
/// <param name="color"></param>
void ModelDraw::SetRimLithgColor(DirectX::XMVECTORF32 color)
{
	m_isRimLight = true;
	m_rimColor = color;
}

/// <summary>
/// エミッシブカラーを設定
/// </summary>
/// <param name="color"></param>
void ModelDraw::SetEmissiveColor(DirectX::XMVECTORF32 color)
{
	m_emissiveColor = color;
}
