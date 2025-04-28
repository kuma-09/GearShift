#pragma once
#include "Component.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

/// <summary>
/// モデル描画コンポーネントクラス
/// </summary>
class ModelDraw : public Component
{
public:
	// コンストラクタ
	ModelDraw();
	// デストラクタ
	~ModelDraw();

	// 初期化処理
	void Initialize(DirectX::Model* model,bool texture = false);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// シャドウマップを作成
	void CreateShadow();

	// リムライトカラーを設定
	void SetRimLithgColor(DirectX::XMVECTORF32 color);
	// エミッシブカラーを設定
	void SetEmissiveColor(DirectX::XMVECTORF32 color);

private:
	Graphics* m_graphics;

	// 描画するモデル
	DirectX::Model* m_model;
	// 描画するワールド座標
	DirectX::SimpleMath::Matrix m_world;
	// テクスチャがあるモデルか
	bool m_isTexture;
	// リムライトを適用するか
	bool m_isRimLight;
	// リムライトカラー
	DirectX::XMVECTORF32 m_rimColor;
	// エミッシブカラー
	DirectX::XMVECTORF32 m_emissiveColor;
};

