#pragma once
#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

#include "Model.h"
#include "Graphics.h"

// Resourcesクラスを定義する
class Resources
{
public:
	// 「砲塔」モデルを取得する
	DirectX::Model* GetTurretModel() { return m_turretModel.get(); }
	// 「砲塔下部」モデルを取得する
	DirectX::Model* GetTurretBottomModel() { return m_turretBottomModel.get(); }
	// 「砲塔下部脚」モデルを取得する
	DirectX::Model* GetTurretBottomFootModel() { return m_turretFootModel.get(); }
	// 「砲塔中部プレート」モデルを取得する
	DirectX::Model* GetTurretMiddlePlateModel() { return m_turretPlateModel.get(); }
	// 「砲塔中部」モデルを取得する
	DirectX::Model* GetTurretMiddleModel() { return m_turretMiddleModel.get(); }
	// 「砲塔上部」モデルを取得する
	DirectX::Model* GetTurretTopModel() { return m_turretTopModel.get(); }
	// 「砲塔砲台」モデルを取得する
	DirectX::Model* GetTurretFortModel() { return m_turretFortModel.get(); }
	// 「砲塔砲身」モデルを取得する
	DirectX::Model* GetTurretGunModel() { return m_turretGunModel.get(); }
	// 「砲弾」モデルを取得する
	DirectX::Model* GetBulletModel() { return m_bulletModel.get(); }

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
		m_turretModel{},								// 砲塔モデル
		m_turretBottomModel{},				// 砲塔下部モデル
		m_turretFootModel{},						// 砲塔脚モデル
		m_turretPlateModel{},						// 砲塔プレートモデル
		m_turretMiddleModel{},					// 砲塔中部モデル
		m_turretTopModel{},						// 砲塔上部モデル
		m_turretFortModel{},						// 砲台モデル
		m_turretGunModel{},						// 砲身モデル
		m_bulletModel{}									// 砲弾モデル
	{
	}

private:
	// リソース
	static std::unique_ptr<Resources> m_resources;
	// グラフィックス
	Graphics* m_graphics;
	// デバイス
	ID3D11Device* m_device;

	// 「砲塔」モデル
	std::unique_ptr<DirectX::Model> m_turretModel;
	// 「砲塔下部」モデル
	std::unique_ptr<DirectX::Model> m_turretBottomModel;
	// 「砲塔脚」モデル
	std::unique_ptr<DirectX::Model> m_turretFootModel;
	// 「砲塔プレート」
	std::unique_ptr<DirectX::Model> m_turretPlateModel;
	// 「砲塔中部」
	std::unique_ptr<DirectX::Model> m_turretMiddleModel;
	// 「砲塔上部」モデル
	std::unique_ptr<DirectX::Model> m_turretTopModel;
	// 「砲台」モデル
	std::unique_ptr<DirectX::Model> m_turretFortModel;
	// 「砲身」モデル
	std::unique_ptr<DirectX::Model> m_turretGunModel;
	// 「砲弾」モデル
	std::unique_ptr<DirectX::Model> m_bulletModel;
};

#endif		// RESOURCES_DEFINED
