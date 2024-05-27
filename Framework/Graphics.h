#pragma once
#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED

#include "DeviceResources.h"

class Graphics final
{
public:
	// デバイスリソースを取得する
	DX::DeviceResources* GetDeviceResources() { return m_deviceResources.get(); }
	// スクリーンサイズを取得する
	void GetScreenSize(int& width, int& height) const { width = m_screenW; height = m_screenH; }
	// スクリーンサイズを設定する
	void SetScreenSize(const int& width, const int& height) { m_screenW = width; m_screenH = height; }
	// PrimitiveBatchクラスのインスタンスを取得する
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* GetPrimitiveBatch() const { return m_primitiveBatch.get(); }
	// PrimitiveBatchクラスのインスタンスを取得する
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetPrimitiveBatchPositionColor() const { return m_primitiveBatchPositionColor.get(); }
	// InputLayoutクラスのインスタンスを取得する
	ID3D11InputLayout* GetInputLayout() const { return m_inputLayout.Get(); }
	// BasicEffectクラスのインスタンス取得する
	DirectX::BasicEffect* GetBasicEffect() { return m_basicEffect.get(); }
	// CommonStatesクラスのインスタンスを取得する
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates.get(); }
	// SpriteBatchクラスのインスタンスを取得する
	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	// SpriteFontクラスのインスタンスを取得する
	DirectX::SpriteFont* GetFont() { return m_spriteFont.get(); }
	// EffectFactoryクラスのインスタンスを取得する
	DirectX::EffectFactory* GetFX() const { return m_effectFactory.get(); }
	// ビュー行列を設定する
	void SetViewMatrix(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
	// ビュー行列を取得する
	const DirectX::SimpleMath::Matrix& GetViewMatrix() { return m_view; };
	// 射影行列を設定する
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }
	// 射影行列を取得する
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() { return m_projection; };

private:
	// コンストラクタ
	Graphics();
	// インスタンスをコピーすることを禁止する
	void operator=(const Graphics&) = delete;
	// インスタンスをムーブすることを禁止する
	Graphics& operator= (Graphics&&) = delete;
	// コピーコンストラクタは禁止する
	Graphics(const Graphics&) = delete;
	// ムーブコンストラクタは禁止する
	Graphics(Graphics&&) = delete;

public:
	static Graphics* GetInstance();

	// 初期化する
	void Initialize();
	// デストラクタ
	~Graphics() = default;

	// プリミティブ描画を開始する
	void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, const DirectX::SimpleMath::Matrix& world);
	void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector4 color);
	// プリミティブ描画を終了する
	void DrawPrimitiveEnd();

	// プリミティブ描画を開始する
	void DrawPrimitivePositionColorBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// プリミティブ描画を終了する
	void DrawPrimitivePositionColorEnd();

private:
	// グラフィックス
	static std::unique_ptr<Graphics> m_graphics;
	// デバイスリソース
	std::unique_ptr<DX::DeviceResources> m_deviceResources;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonStates;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatchPositionColor;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_effectFactory;
	// ラスタライザーステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterrizerState;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// スクリーンサイズ
	int m_screenW, m_screenH;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// デバイス
	ID3D11Device* m_device;
	// デバイスコンテキスト
	ID3D11DeviceContext* m_context;
};
#endif		// GRAPHICS_DEFINED

