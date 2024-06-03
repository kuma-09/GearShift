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
	DirectX::Model* GetPlayerModel()	 { return m_player.get(); }
	DirectX::Model* GetHeadModel()		 { return m_head.get(); }
	DirectX::Model* GetBodyTopModel()	 { return m_bodyTop.get(); }
	DirectX::Model* GetBodyBottomModel() { return m_bodyBottom.get(); }
	DirectX::Model* GetlArmModel()		 { return m_lArm.get(); }
	DirectX::Model* GetrArmModel()		 { return m_rArm.get(); }
	DirectX::Model* GetlLegModel()		 { return m_lLeg.get(); }
	DirectX::Model* GetrLegModel()		 { return m_rLeg.get(); }
	DirectX::Model* GetDiceModel()		 { return m_dice.get(); }
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
		m_player{},											// モデル
		m_dice{},
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
	std::unique_ptr<DirectX::Model> m_player;
	std::unique_ptr<DirectX::Model> m_head;
	std::unique_ptr<DirectX::Model> m_bodyTop;
	std::unique_ptr<DirectX::Model> m_bodyBottom;
	std::unique_ptr<DirectX::Model> m_lArm;
	std::unique_ptr<DirectX::Model> m_rArm;
	std::unique_ptr<DirectX::Model> m_lLeg;
	std::unique_ptr<DirectX::Model> m_rLeg;
	std::unique_ptr<DirectX::Model> m_dice;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

#endif		// RESOURCES_DEFINED
