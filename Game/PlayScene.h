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
#include "Game/Object/DropItemB.h"
#include "Game/Object/SkyDome.h"
#include "Game/Object/Floor.h"

#include "Game/Player/TargetArea.h"
#include "Shader/PostProcess.h"

#include "Framework/DebugString.h"

class Camera;
class HPUI;
class HitParticle;
class HitEffect;
class StartAnimation;
class BulletMagazine;
class ExBulletMagazine;


class PlayScene final: public IScene
{
public:
	PlayScene();
	~PlayScene();

	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;

	void UpdateBulletMagazine();

	// ヒットエフェクトを生成する関数
	void SetNoise();
	void CreateHitParticle(DirectX::SimpleMath::Matrix world);
	void CreateHitParticle(DirectX::SimpleMath::Matrix world,DirectX::SimpleMath::Quaternion rotate);
	void CreateHitEffect(DirectX::SimpleMath::Vector3 pos);
private:

	void CreateShadow();

	// 敵を削除
	std::vector<std::unique_ptr<Enemy>>::iterator RemoveEnemy(std::vector<std::unique_ptr<Enemy>>::iterator it);

	// アイテムを削除
	void RemoveItem(std::vector<std::unique_ptr<DropItem>>::iterator it);
	void RemoveItemB(std::vector<std::unique_ptr<DropItemB>>::iterator it);

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
	std::vector<std::unique_ptr<DropItemB>> m_dropItemB;
	// スカイドーム
	std::unique_ptr<SkyDome> m_skyDome;
	// 床
	std::vector<std::unique_ptr<Floor>>   m_floor;
	// ターゲット範囲
	std::unique_ptr<TargetArea> m_targetArea;
	// HPの表示UI
	std::unique_ptr<HPUI> m_hpUI;
	// ヒットパーティクル
	std::vector<std::unique_ptr<HitParticle>> m_hitParticle;
	// 撃破エフェクト
	std::unique_ptr<HitEffect> m_hitEffect;


	std::unique_ptr<StartAnimation> m_startAnimation;
	std::unique_ptr<BulletMagazine> m_bulletMagazine;
	std::unique_ptr<ExBulletMagazine> m_exBulletMagazine;

	// 経過時間
	float m_totalTime;
	// 残り時間
	float m_timeLimit;
	
	std::unique_ptr<PostProcess> m_postProcess;

};