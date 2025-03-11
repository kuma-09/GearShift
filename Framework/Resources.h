#pragma once

#include "Model.h"
#include "Graphics.h"
#include <unordered_map>

/// <summary>
/// モデルデータを管理するクラス
/// シングルトン
/// </summary>
class Resources
{
public:

	// モデルの種類
	enum ModelType
	{
		Player,
		Head,
		Body,
		LArm,
		RArm,
		LLeg,
		RLeg,


		HomingEnemy,
		TankBody,
		BossLeg,
		BossHead,

		Cube,
		SkyDome,
		Floor,
		Floor2,
		BillA,
		BillB,
		BillC,

		Cloud1,
		Cloud2,
		Cloud3,

		DropItem,

		Boom,

		Gun,
		MissileLuncher,

		Light,
	};

	// モデルを取得する
	DirectX::Model* GetModel(ModelType type);

	// テクスチャを取得
	ID3D11ShaderResourceView* GetWhiteTexture() { return m_Texture.Get(); }

public:
	Resources(Resources&&) = default;
	Resources& operator= (Resources&&) = default;
	Resources& operator= (Resources const&) = delete;
	// デストラクタ
	~Resources();
	// Resoucesクラスのインスタンスを取得する
	static Resources* const GetInstance();

	// リソースを読み込む
	void LoadResource(Graphics* graphics);

private:
	// コンストラクタ
	Resources() noexcept
	{
	}

private:
	// リソース
	static std::unique_ptr<Resources> m_resources;

	// モデル
	std::unordered_map<ModelType, std::unique_ptr<DirectX::Model>> m_umModel;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;

};
