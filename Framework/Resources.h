#pragma once
#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

#include "Model.h"
#include "Graphics.h"

// Resourcesクラスを定義する
class Resources
{
public:
	// モデルを取得する
	DirectX::Model* GetModel() { return m_model.get(); }
	// テクスチャを取得
	ID3D11ShaderResourceView* GetTexture() { return m_texture.Get(); }
public:
	Resources(Resources&&) = default;
	Resources& operator= (Resources&&) = default;
	Resources& operator= (Resources const&) = delete;
	// デストラクタ
	~Resources() = default;
	// Resoucesクラスのインスタンスを取得する
	static Resources* const GetInstance();

	void Initalize(Graphics* graphics);

	// リソースをロードする
	void LoadResource();

private:
	// コンストラクタ
	Resources() noexcept
		:
		m_graphics{},										// グラフィックス
		m_device{},											// デバイス
		m_model{},											// モデル
		m_texture{}											// テクスチャ

	{
	}

private:
	// リソース
	static std::unique_ptr<Resources> m_resources;
	// グラフィックス
	Graphics* m_graphics;
	// デバイス
	ID3D11Device* m_device;

	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

#endif		// RESOURCES_DEFINED
