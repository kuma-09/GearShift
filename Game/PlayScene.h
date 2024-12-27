#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Particle/Emitter.h"
#include "Game/Object/DropItem.h"
#include "Game/Object/DropItemB.h"
#include "Game/Object/SkyDome.h"
#include "Game/Object/Floor.h"
#include "Game/Player/Player.h"
#include "Game/Player/TargetArea.h" 
#include "Shader/PostProcess.h"
#include "Framework/DebugString.h"

class Camera;
class Reload;
class HitParticle;
class ExplosionEffect;
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
	void TranslucentRender() override;
	void RenderUI() override;
	void Finalize() override;

	// ヒットエフェクトを生成する関数
	void SetNoise();
	void CreateHitParticle(DirectX::SimpleMath::Matrix world);
	void CreateHitParticle(DirectX::SimpleMath::Matrix world,DirectX::SimpleMath::Quaternion rotate);
	void CreateHitEffect(DirectX::SimpleMath::Vector3 pos);
private:

	void CreateObject(std::string className, DirectX::SimpleMath::Vector3 pos);

private:

	
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;


	// ゲームオブジェクトをまとめたポインタ配列
	std::vector<GameObject*> m_gameObjects;
	// プレイヤー
	std::weak_ptr<GameObject> m_player;
	// アイテム配列
	std::vector<std::unique_ptr<DropItem>> m_dropItem;
	std::vector<std::unique_ptr<DropItemB>> m_dropItemB;
	// スカイドーム
	std::unique_ptr<SkyDome> m_skyDome;
	// 床
	std::vector<std::unique_ptr<Floor>>   m_floor;
	// ターゲット範囲
	std::unique_ptr<TargetArea> m_targetArea;

	std::vector<std::unique_ptr<Bullet>> m_bullets;

	// ヒットパーティクル
	std::vector<std::unique_ptr<HitParticle>> m_hitParticle;
	// 撃破エフェクト
	std::unique_ptr<ExplosionEffect> m_hitEffect;


	std::unique_ptr<StartAnimation> m_startAnimation;
	std::unique_ptr<BulletMagazine> m_bulletMagazine;
	std::unique_ptr<ExBulletMagazine> m_exBulletMagazine;

	// 経過時間
	float m_totalTime;
	// 残り時間
	float m_timeLimit;
	
	std::unique_ptr<PostProcess> m_postProcess;

};