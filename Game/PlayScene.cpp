#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Game.h"

#include "Framework/Microsoft/DebugDraw.h"
#include "Framework/Audio.h"
#include "Framework/Json.h"

#include "Player/Player.h"
#include "Enemy/FixedEnemy.h"
#include "Enemy/HomingEnemy.h"
#include "Enemy/BossEnemy.h"
#include "Game/Object/Wall/BillA.h"
#include "Game/Object/Wall/BillB.h"
#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Light/Light.h"

#include "Game/Components/HPBar.h"

#include "Game/Particle/HitParticle.h"

#include "Game/Animation/StartAnimation.h"
#include "UI/BulletMagazine.h"
#include "UI/ExBulletMagazine.h"

#include "System/HitStop.h"

#include "Manager/RenderManager.h"
#include "Manager/ObjectManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/StageDataManager.h"

PlayScene::PlayScene()
    :
    m_deviceResources{},
    m_graphics{},
    m_inputManager{},
    m_timeLimit{},
    m_totalTime{}
{
}

PlayScene::~PlayScene()
{
    //Finalize();
}

/// <summary> 初期化処理 </summary>
/// <param name="game"> ゲーム </param>
void PlayScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    SetGame(game);

    // 制限時間
    m_timeLimit = 180.0f;
    m_totalTime = 0;

    m_time = std::make_unique<Number>();
    m_time->Initialize({ 640,30 });

    // JsonファイルのDataを読み込み
    std::wstring fileName = StageDataManager::GetStageFileName();
    std::vector<std::string> str;
    std::vector<Vector3> pos;
    Json::LoadJsonFile(fileName, str, pos);

    // オブジェクトの生成
    for (int i = 0; i < str.size(); i++)
    {
        CreateObject(str[i], pos[i]);
    }

    m_skyDome = std::make_unique<SkyDome>();
    m_skyDome->Initialize(Vector3::Zero);

    m_targetArea = std::make_unique<TargetArea>();
    m_targetArea->Initialize();

    m_hitEffect = std::make_unique<ExplosionEffect>();
    m_hitEffect->Initialize();

    for (int i = 0; i < 128; i++)
    {
        m_hitParticle.emplace_back(std::make_unique<HitParticle>());
    }

    m_startAnimation = std::make_unique<StartAnimation>();
    m_startAnimation->Initialize();
    
}

/// <summary> 更新処理 </summary>
/// <param name="elapsedTime"> 経過時間 </param>
void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;
    ObjectManager::Delete();

    HitStop::Update(elapsedTime);
    if (HitStop::GetIsStop()) return;

    // 経過時間を計算
    m_totalTime += elapsedTime;
    m_time->SetNum(int(m_totalTime));
    m_startAnimation->Update(elapsedTime);

    // ヒットエフェクトの更新
    UpdateParticle(elapsedTime);
    // ターゲットエリアの更新
    UpdateTargetArea();
    // オブジェクトの更新
    ObjectManager::Update(elapsedTime);
    CollisionManager::Update();

}

// 3Dオブジェクトの描画
void PlayScene::Render()
{
    RenderManager::RenderObjects();
}

// 半透明オブジェクトの描画
void PlayScene::TranslucentRender()
{
    RenderManager::RenderParticle();
    for (auto& particle : m_hitParticle)
    {
        particle->Render(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    }
    m_hitEffect->Render();
}

// UIの描画
void PlayScene::RenderUI()
{
    m_time->RenderTime();
    m_targetArea->Render(m_targetArea->GetTarget());
    static_cast<Player*>(m_player.lock().get())->RenderPlayerUI();
    m_startAnimation->Render();
    if (m_targetArea->GetTarget())
    {
        m_targetArea->GetTarget()->GetComponent<HPBar>()->Render(m_targetArea->GetTarget()->GetPosition());
    }
}

/// <summary> 終了処理 </summary>
void PlayScene::Finalize()
{
    ObjectManager::Clear();
    RenderManager::Clear();
}

/// ヒットエフェクトを生成する関数
void PlayScene::CreateHitParticle(DirectX::SimpleMath::Vector3 pos , float size)
{
    using namespace DirectX::SimpleMath;

    int particleValue = HitParticle::get_rand(1, 5);

    for (int i = 0; i < particleValue; i++)
    {
        float velocityX = (float)HitParticle::get_rand(-30, 30) / 500.0f;
        float velocityY = (float)HitParticle::get_rand(-30, 30) / 500.0f;
        float velocityZ = (float)HitParticle::get_rand(-30, 30) / 500.0f;
        for (auto& particle : m_hitParticle)
        {
            if (particle->GetAlpha() > 0.0f) continue;
            particle->Initialize(pos, Vector3(velocityX, velocityY, velocityZ),{size,size});
            break;
        }
    }
}

// 爆発エフェクトの生成
void PlayScene::CreateHitEffect(DirectX::SimpleMath::Vector3 pos)
{
    m_hitEffect->Set(pos);
}

// Jsonから読み取った座標にオブジェクトを生成
void PlayScene::CreateObject(std::string className, DirectX::SimpleMath::Vector3 pos)
{
    using namespace DirectX::SimpleMath;
    if (className == "Player")
    {
        m_player = ObjectManager::Add(std::make_shared<Player>(this),pos);
    }
    if (className == "Floor")
    {
        ObjectManager::Add(std::make_shared<Floor>(this), pos);
    }
    if (className == "BillA")
    {
        ObjectManager::Add(std::make_shared<BillA>(this),pos);
    }
    if (className == "BillB")
    {
        ObjectManager::Add(std::make_shared<BillB>(this),pos);
    }
    if (className == "HomingEnemy")
    {
        ObjectManager::Add(std::make_shared<HomingEnemy>(this,m_player.lock().get()),pos, Type::Enemy);
    }
    if (className == "FixedEnemy")
    {
        ObjectManager::Add(std::make_shared<FixedEnemy>(this, m_player.lock().get()),pos, Type::Enemy);
    }
    if (className == "BossEnemy")
    {
        ObjectManager::Add(std::make_shared<BossEnemy>(this, m_player.lock().get()),pos, Type::Enemy);
    }
    if (className == "DropItem")
    {
        ObjectManager::Add(std::make_shared<DropItem>(this), pos);
    }
    if (className == "DropItemB")
    {
        ObjectManager::Add(std::make_shared<DropItemB>(this),pos);
    }   
    if (className == "Light")
    {
        ObjectManager::Add(std::make_shared<Light>(this), pos);
    }
}

// ターゲットエリアの更新
void PlayScene::UpdateTargetArea()
{
    m_targetArea->ClearTarget();
    auto enemys = ObjectManager::GetTypeObjects(Type::Enemy);
    if (enemys.empty())
    {
        GetGame()->ChangeScene(GetGame()->GetResultScene());
    }

    for (auto& enemy : enemys)
    {
        m_targetArea->Update(m_player.lock().get(), enemy.lock().get(), static_cast<Player*>(m_player.lock().get())->GetCamera());
    }

    static_cast<Player*>(m_player.lock().get())->SetTarget(m_targetArea->GetTarget());
}

// パーティクルの更新
void PlayScene::UpdateParticle(float elapsedTime)
{
    for (auto& particle : m_hitParticle)
    {
        particle->Update();
    }
    m_hitEffect->Update(elapsedTime);
}