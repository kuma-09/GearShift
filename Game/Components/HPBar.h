#pragma once
#include "Component.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

/// <summary>
/// HPを描画するコンポーネントクラス
/// </summary>
class HPBar : public Component
{
public:
	// コンストラクタ
	HPBar();
	// デストラクタ
	~HPBar();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理(2D空間
	void Render(DirectX::SimpleMath::Vector2 position);
	// 描画処理(3D空間
	void Render(DirectX::SimpleMath::Vector3 position);

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// HPの最大値
	int  m_maxHp;
	// 残りのHP
	float m_greenHp;
	// 減少中のHP
	float m_redHp;
	// 経過時間
	float m_nowTime;

	// HP描画用の四頂点
	DirectX::VertexPositionColorTexture m_vertex[4];
	DirectX::VertexPositionColorTexture m_redVertex[4];
	DirectX::VertexPositionColorTexture m_backVertex[4];

	// テクスチャ
	ID3D11ShaderResourceView* m_texture;

};

