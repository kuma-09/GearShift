#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Game.h"

#include "Framework/Audio.h"
#include "Framework/Json.h"

#include "Player/Player.h"
#include "Enemy/TrainingEnemy.h"
#include "Enemy/FixedEnemy.h"
#include "Enemy/HomingEnemy.h"
#include "Enemy/BossEnemy.h"
#include "Game/Object/SkyDome.h"
#include "Game/Object/Wall/BillA.h"
#include "Game/Object/Wall/BillB.h"
#include "Game/Object/Wall/BillC.h"
#include "Game/Object/Wall/BillD.h"
#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Light/Light.h"
#include "Game/Object/Cloud/Cloud1.h"
#include "Game/Object/Cloud/Cloud2.h"
#include "Game/Object/Cloud/Cloud3.h"

#include "Game/Particle/HitParticle.h"

#include "System/HitStop.h"

#include "Manager/RenderManager.h"
#include "Manager/ObjectManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/StageDataManager.h"

PlayScene::PlayScene()
    :
    m_graphics{},
    m_inputManager{},
    m_totalTime{},
    m_isMenu{},
    m_isTutorial{}
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
    m_inputManager = InputManager::GetInstance();

    SetGame(game);

    // 経過時間
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

    CreateMenu();

    m_targetArea = std::make_unique<TargetArea>();
    m_targetArea->Initialize();

    m_hitEffect = std::make_unique<ExplosionEffect>();
    m_hitEffect->Initialize();

    for (int i = 0; i < 128; i++)
    {
        m_hitParticle.emplace_back(std::make_unique<HitParticle>());
    }


    Audio::GetInstance()->ChageBGM(Audio::Battle);
}

/// <summary> 更新処理 </summary>
/// <param name="elapsedTime"> 経過時間 </param>
void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;
    ObjectManager::Delete();
    HitStop::Update(elapsedTime);
    if (HitStop::GetIsStop()) return;

    auto& kb = m_inputManager->GetKeyboardTracker();

    if (kb->IsKeyPressed(DirectX::Keyboard::Escape)) m_isMenu = true;

    if (m_isMenu)
    {
        UpdateMenu();
        return;
    }

    // 経過時間を計算
    m_totalTime += elapsedTime;
    m_tutorial->Update(elapsedTime);
    m_time->SetNum(int(m_totalTime));
    m_startAnimation->Update(elapsedTime);
    if (m_finishAnimation->Update(elapsedTime)) GetGame()->ChangeScene(GetGame()->GetResultScene());

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

    auto view = m_graphics->GetViewMatrix();
    auto proj = m_graphics->GetProjectionMatrix();

    RenderManager::RenderParticle();
    for (auto& particle : m_hitParticle)
    {
        particle->Render(view,proj);
    }
    m_hitEffect->Render();
}

// UIの描画
void PlayScene::RenderUI()
{
    using namespace DirectX::SimpleMath;

    m_time->RenderTime();
    m_targetArea->Render(m_targetArea->GetTarget());
    static_cast<Player*>(m_player.lock().get())->RenderPlayerUI();
    m_tutorial->Render();
    m_startAnimation->Render();
    m_finishAnimation->Render();
    m_remainingEnemyUI->Render();
    if (m_isMenu)
    {
        m_menuBack->Render(Vector2{640,360 }, DirectX::XMVECTORF32({ 1,1,1,0.75f }), Vector2(640, 360), Vector2(0.9f, 0.9f));
        m_menu->Render({ 640,0 });
    }
}

/// <summary> 終了処理 </summary>
void PlayScene::Finalize()
{
    Audio::GetInstance()->ChageBGM(Audio::Title);
    StageDataManager::SetClearTime(m_totalTime);
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
    if (className == "Player")  m_player = ObjectManager::Add(std::make_shared<Player>(this),pos);
    if (className == "SkyDome")            ObjectManager::Add(std::make_shared<SkyDome>(), pos);
    if (className == "Floor")              ObjectManager::Add(std::make_shared<Floor>(this), pos);
    if (className == "Floor2")             ObjectManager::Add(std::make_shared<Floor2>(this), pos);
    if (className == "Cloud1")             ObjectManager::Add(std::make_shared<Cloud1>(), pos);
    if (className == "Cloud2")             ObjectManager::Add(std::make_shared<Cloud2>(), pos);
    if (className == "Cloud3")             ObjectManager::Add(std::make_shared<Cloud3>(), pos);
    if (className == "BillA")              ObjectManager::Add(std::make_shared<BillA>(this),pos);
    if (className == "BillB")              ObjectManager::Add(std::make_shared<BillB>(this),pos);
    if (className == "BillC")              ObjectManager::Add(std::make_shared<BillC>(this), pos);
    if (className == "BillD")              ObjectManager::Add(std::make_shared<BillD>(this), pos);
    if (className == "TrainingEnemy")      ObjectManager::Add(std::make_shared<TrainingEnemy>(this), pos,Type::Enemy);
    if (className == "HomingEnemy")        ObjectManager::Add(std::make_shared<HomingEnemy>(this,m_player.lock().get()),pos, Type::Enemy);
    if (className == "FixedEnemy")         ObjectManager::Add(std::make_shared<FixedEnemy>(this, m_player.lock().get()),pos, Type::Enemy);
    if (className == "BossEnemy")          ObjectManager::Add(std::make_shared<BossEnemy>(this, m_player.lock().get()),pos, Type::Enemy);
    if (className == "DropItem")           ObjectManager::Add(std::make_shared<DropItem>(this), pos);
    if (className == "DropItemB")          ObjectManager::Add(std::make_shared<DropItemB>(this),pos);
    if (className == "Light")              ObjectManager::Add(std::make_shared<Light>(this), pos);
}

void PlayScene::CreateMenu()
{
    // ポーズした時のメニュー
    m_menu = std::make_unique<Menu>();
    m_menu->AddUI(L"Resources/Textures/Resume.png", { 0,150 }, { 1.f,1.f });
    //m_menu->AddUI(L"Resources/Textures/Option.png", { 0,350 }, { 1.f,1.f });
    m_menu->AddUI(L"Resources/Textures/Exit.png", { 0,350 }, { 1.f,1.f });
    m_menu->Initialize();
    m_isMenu = false;
    m_menuBack = std::make_unique<UI>(L"Resources/Textures/SceneChangeBlack.png");
    m_menuBack->Initialize();

    // 開始終了のアニメーション
    m_startAnimation = std::make_unique<StartAnimation>();
    m_startAnimation->Initialize();
    m_finishAnimation = std::make_unique<FinishAnimation>();

    // チュートリアルUI
    m_tutorial = std::make_unique<Tutorial>();
    m_tutorial->Initialize(StageDataManager::GetStageNum());

    // 残りの敵の数UI
    auto enemys = ObjectManager::GetTypeObjects(Type::Enemy);
    m_remainingEnemyUI = std::make_unique<RemainingEnemyUI>();
    m_remainingEnemyUI->Initialize({ 1150,30 }, enemys.size());
}

// ターゲットエリアの更新
void PlayScene::UpdateTargetArea()
{
    m_targetArea->ClearTarget();
    auto enemys = ObjectManager::GetTypeObjects(Type::Enemy);
    if (enemys.empty())
    {
        m_finishAnimation->Initialize();
    }
    m_remainingEnemyUI->Update(enemys.size());
    auto player = static_cast<Player*>(m_player.lock().get());

    for (auto& enemy : enemys)
    {
        m_targetArea->Update(player, enemy.lock().get(), player->GetCamera());
    }
    player->SetTarget(m_targetArea->GetTarget());;
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

void PlayScene::UpdateMenu()
{

    auto& kb = m_inputManager->GetKeyboardTracker();

    m_menu->Update();
    if (kb->pressed.Space)
    {
        switch (m_menu->GetActiveUI())
        {
        case 0:
            m_isMenu = false;
            break;
        case 1:
            GetGame()->ChangeScene(GetGame()->GetTitleScene());
            break;
        case 2:
            GetGame()->ChangeScene(GetGame()->GetTitleScene());
            break;
        default:
            break;
        }
    }
}
