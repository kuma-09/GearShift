#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Particle/Emitter.h"
#include "Player/Player.h"
#include "Enemy/FixedEnemy.h"
#include "Enemy/HomingEnemy.h"
#include "Game/Object/Wall/Wall.h"
#include "Game/Object/DropItem.h"
#include "Game/Object/SkyDome.h"
#include "Game/Object/Floor.h"

#include "Game/Player/TargetArea.h"

#include "Framework/DebugString.h"

class Camera;
class HPUI;
class HitParticle;


class PlayScene final: public IScene
{
public:
	PlayScene();
	~PlayScene();

	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;

	// ボックスコライダーを追加
	void AddCollider(BoxCollider* collider)
	{
		m_pBoxCollider.push_back(collider);
	}

	// コライダーを削除する
	void RemoveCollider(BoxCollider* collider)
	{
		int it = 0;
		for (auto boxColliders : m_pBoxCollider)
		{
			if (boxColliders == collider)
			{
				break;
			}
			it++;
		}
		m_pBoxCollider.erase(m_pBoxCollider.begin() + it);
	}

	// ボックスコライダーの配列を取得
	std::vector<BoxCollider*> GetColliders() { return m_pBoxCollider; }

	// ヒットエフェクトを生成する関数
	void CreateHitParticle(DirectX::SimpleMath::Matrix world);

private:


	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	// プレイヤー
	std::unique_ptr<Player> m_player;
	// エネミー配列
	std::vector<std::unique_ptr<Enemy>> m_Enemy;
	// 壁配列
	std::vector<std::unique_ptr<Wall>> m_wall;
	// アイテム配列
	std::vector<std::unique_ptr<DropItem>> m_dropItem;
	// スカイドーム
	std::unique_ptr<SkyDome> m_skyDome;
	// 床
	std::vector<std::unique_ptr<Floor>>   m_floor;

	// ターゲット範囲
	std::unique_ptr<TargetArea> m_targetArea;

	// HPの表示UI
	std::unique_ptr<HPUI> m_hpUI;


	// ボックスコライダーをまとめた配列
	std::vector<BoxCollider*> m_pBoxCollider;

	// デバッグ文字
	std::unique_ptr<DebugString> m_debugString;

	// ヒットエフェクト
	std::vector<std::unique_ptr<HitParticle>> m_hitParticle;

	// 経過時間
	float m_totalTime;
	// 残り時間
	float m_timeLimit;
	
};