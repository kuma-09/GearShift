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
#include "Game/Object/Floor/Floor2.h"
#include "Game/Player/Player.h"
#include "Game/Player/TargetArea.h" 
#include "Shader/PostProcess.h"
#include "Framework/DebugString.h"
#include "UI/Number.h"
#include "UI/Menu.h"
#include "UI/RemainingEnemyUI.h"

#include "Game/Animation/StartAnimation.h"
#include "Game/Animation/FinishAnimation.h"

#include "Game/System/Tutorial.h"

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

	void CreateHitParticle(DirectX::SimpleMath::Vector3 pos,float size = 1.0f);
	void CreateHitEffect(DirectX::SimpleMath::Vector3 pos);
private:
	void CreateObject(std::string className, DirectX::SimpleMath::Vector3 pos);
	void CreateMenu();
	void UpdateTargetArea();
	void UpdateParticle(float elapsedTime);
	void UpdateMenu();
private:

	
	Graphics* m_graphics;
	InputManager* m_inputManager;


	// ゲームオブジェクトをまとめたポインタ配列
	std::vector<GameObject*> m_gameObjects;
	// プレイヤー
	std::weak_ptr<GameObject> m_player;
	// スカイドーム
	std::unique_ptr<SkyDome> m_skyDome;
	// ターゲット範囲
	std::unique_ptr<TargetArea> m_targetArea;

	// ヒットパーティクル
	std::vector<std::unique_ptr<HitParticle>> m_hitParticle;
	// 撃破エフェクト
	std::unique_ptr<ExplosionEffect> m_hitEffect;
	
	// ミッション開始時と終了時のアニメーション
	std::unique_ptr<StartAnimation> m_startAnimation;
	std::unique_ptr<FinishAnimation> m_finishAnimation;
	// ポーズ中のメニュー
	std::unique_ptr<Menu> m_menu;
	// メニューの背景
	std::unique_ptr<UI>   m_menuBack;
	// チュートリアル
	std::unique_ptr<Tutorial> m_tutorial;
	// 敵の残り数描画クラス
	std::unique_ptr<RemainingEnemyUI> m_remainingEnemyUI;
	// 経過時間描画クラス
	std::unique_ptr<Number> m_time;

	// 経過時間
	float m_totalTime;
	// メニューを開いてるか
	bool  m_isMenu;
	// チュートリアル
	bool  m_isTutorial;
};