#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Game.h"

#include "Framework/Microsoft/DebugDraw.h"
#include "Framework/Audio.h"
#include "Framework/Json.h"
#include "Game/Components/HP.h"
#include "Game/Components/Camera.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"


#include "Game/Object/Wall/BillA.h"
#include "Game/Object/Wall/BillB.h"
#include "Game/Object/Bullet/HomingBullet.h"

#include "Game/Particle/HitParticle.h"

#include "UI/HPUI.h"
#include "Game/Animation/StartAnimation.h"
#include "UI/BulletMagazine.h"
#include "UI/ExBulletMagazine.h"


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
    Finalize();
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

    // JsonファイルのDataを読み込み
    std::vector<std::string> str;
    std::vector<Vector3> pos;
    Json::LoadJsonFile(L"Stage1.json", str, pos);

    // オブジェクトの生成
    for (int i = 0; i < str.size(); i++)
    {
        CreateObject(str[i], pos[i]);
    }


    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Bullet>> bullets2;

    for (int i = 0; i < 10; i++)
    {
        bullets.emplace_back(std::make_unique<HomingBullet>(this, BoxCollider::PlayerBullet));
        bullets2.emplace_back(std::make_unique<HomingBullet>(this, BoxCollider::PlayerBullet));
    }

    m_dropItemB.emplace_back(std::make_unique<DropItemB>(this, std::move(bullets)));
    m_dropItemB.back()->SetPosition(Vector3(10, 3, 10));

    m_dropItemB.emplace_back(std::make_unique<DropItemB>(this, std::move(bullets2)));
    m_dropItemB.back()->SetPosition(Vector3(10, 30, 30));

    m_floor.emplace_back(std::make_unique<Floor>(this));
    m_floor.back()->SetPosition({ -100,0,-100 });
    m_floor.emplace_back(std::make_unique<Floor>(this));
    m_floor.back()->SetPosition({  100,0,-100 });
    m_floor.emplace_back(std::make_unique<Floor>(this));
    m_floor.back()->SetPosition({ -100,0, 100 });
    m_floor.emplace_back(std::make_unique<Floor>(this));
    m_floor.back()->SetPosition({  100,0, 100 });

    m_skyDome = std::make_unique<SkyDome>();
    m_skyDome->Initialize(Vector3::Zero);

    m_targetArea = std::make_unique<TargetArea>();
    m_targetArea->Initialize();

    m_postProcess = std::make_unique<PostProcess>();
    m_postProcess->Initialize();

    m_hpUI = std::make_unique<HPUI>();

    m_hitEffect = std::make_unique<HitEffect>();
    m_hitEffect->Initialize();

    m_startAnimation = std::make_unique<StartAnimation>();
    m_startAnimation->Initialize();
    m_bulletMagazine = std::make_unique<BulletMagazine>();
    m_bulletMagazine->Initialize(10);
    m_exBulletMagazine = std::make_unique<ExBulletMagazine>();
    m_exBulletMagazine->Initialize(0);
    
}

/// <summary> 更新処理 </summary>
/// <param name="elapsedTime"> 経過時間 </param>
void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    // 経過時間を計算
    m_totalTime += elapsedTime;
    m_postProcess->Update(elapsedTime);

    m_startAnimation->Update(elapsedTime);
    m_targetArea->ClearTarget();

    // ヒットエフェクトの更新
    for (auto it = m_hitParticle.begin(); it != m_hitParticle.end();)
    {
        if (it->get()->Update())
        {
            it = m_hitParticle.erase(it);
        }
        else
        {
            it++;
        }
    }
    m_hitEffect->Update(elapsedTime);

    // プレイヤーの更新
    m_player->Update(elapsedTime);

    for (auto& dropItem : m_dropItem)
    {
        dropItem->Update(elapsedTime);
    }
    for (auto& dropItem : m_dropItemB)
    {
        dropItem->Update(elapsedTime);
    }
    for (auto& floor : m_floor)
    {
        floor->Update(elapsedTime);
    }
    for (auto& enemy : m_Enemy)
    {
        enemy->Update(elapsedTime);
        m_targetArea->Update(m_player.get(), enemy.get());
    }

    m_player->SetTarget(m_targetArea->GetTarget());

    std::vector<Enemy*> inAreaEnemy;

    // 体力の無い敵を削除
    for (auto it = m_Enemy.begin(); it != m_Enemy.end();)
    {
        if (it->get()->GetComponent<HP>()->GetHP() > 0)
        {
            it++;
        }
        else
        {
            CreateHitEffect(it->get()->GetPosition());
            it = RemoveEnemy(it);
            if (m_Enemy.empty())
            {
                GetGame()->ChangeScene(GetGame()->GetResultScene());
            }
        }
    }

    // 当たり判定
    for (auto& collider : GetColliders())
    {
        for (auto& enemyCollider : GetColliders())
        {
            if (collider->GetBoundingBox()->Intersects(*enemyCollider->GetBoundingBox()))
            {
                collider->GetOwner()->Collision(enemyCollider);
            }
        }
    }

    // プレイヤーがドロップアイテムに触れている時
    for (auto it = m_dropItem.begin(); it != m_dropItem.end(); it++)
    {
        if (m_player->GetComponent<BoxCollider>()->
            GetBoundingBox()->Intersects(
                *it->get()->GetComponent<BoxCollider>()->GetBoundingBox()))
        {
            it->get()->SetHit(true);
            Audio::GetInstance()->PlaySoundSE_PowerUp();
            RemoveItem(it);
            break;
        }
        else it->get()->SetHit(false);
    }

    // プレイヤーがドロップアイテムに触れている時
    for (auto it = m_dropItemB.begin(); it != m_dropItemB.end(); it++)
    {
        if (m_player->GetComponent<BoxCollider>()->
            GetBoundingBox()->Intersects(
                *it->get()->GetComponent<BoxCollider>()->GetBoundingBox()))
        {
            it->get()->SetHit(true);
            Audio::GetInstance()->PlaySoundSE_PowerUp();
            m_player->AddWepon(it->get()->GetPart());
            UpdateBulletMagazine();
            RemoveItemB(it);
            break;
        }
        else it->get()->SetHit(false);
    }
    

    // HP表示用のUI
    std::vector<float> hp;
    hp.emplace_back(m_player->GetPart(Part::Head)->GetComponent<HP>()->GetHP() / m_player->GetPart(Part::Head)->GetMaxHP());
    hp.emplace_back(m_player->GetPart(Part::BodyTop)->GetComponent<HP>()->GetHP() / m_player->GetPart(Part::BodyTop)->GetMaxHP());
    hp.emplace_back(m_player->GetPart(Part::LeftArm)->GetComponent<HP>()->GetHP() / m_player->GetPart(Part::LeftArm)->GetMaxHP());
    hp.emplace_back(m_player->GetPart(Part::RightArm)->GetComponent<HP>()->GetHP() / m_player->GetPart(Part::RightArm)->GetMaxHP());
    hp.emplace_back(m_player->GetPart(Part::LeftLeg)->GetComponent<HP>()->GetHP() / m_player->GetPart(Part::LeftLeg)->GetMaxHP());
    hp.emplace_back(m_player->GetPart(Part::RightLeg)->GetComponent<HP>()->GetHP() / m_player->GetPart(Part::RightLeg)->GetMaxHP());

    m_hpUI->Update(hp);

}

/// <summary> 描画処理 </summary>
void PlayScene::Render()
{
    using namespace DirectX;

    // シャドウマップ作成
    CreateShadow();

    // ポストプロセス無しでの描画
    m_postProcess->BeginNormal();

    m_skyDome->Render();

    for (auto& wall : m_wall)
    {
        wall->Render();
    }
    for (auto& floor : m_floor)
    {
        floor->Render();
    }

    for (auto& dropItem : m_dropItem)
    {
        dropItem->Render();
    }
    for (auto& dropItem : m_dropItemB)
    {
        dropItem->Render();
    }
    if (m_player->GetTarget())
    {
        m_player->GetTarget()->GetComponent<HPBar>()->Render(m_player->GetTarget()->GetPosition());
    }

    m_player->Render();
    for (auto& enemy : m_Enemy)
    {
        enemy->Render();
    }

    m_player->RenderState();
    for (auto& particle : m_hitParticle)
    {
        particle->Render(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    }
    m_hitEffect->Render();

    // Bloom-------------------------------------

    m_postProcess->BeginBloom();
    for (auto& dropItem : m_dropItem)
    {
        dropItem->Render();
    }
    for (auto& dropItem : m_dropItemB)
    {
        dropItem->Render();
    }
    m_player->RenderState();
    for (auto& particle : m_hitParticle)
    {
        particle->Render(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    }
    m_hitEffect->Render();

    // リソースの解除＆ライトをキューブで描画
    Resources::GetInstance()->GetShadow()->End();
    //-------------------------------------------

    m_postProcess->combinationRT();

    // UI
    m_targetArea->Render(m_player->GetTarget());
    m_hpUI->Render();
    m_player->RenderPlayerUI();
    m_bulletMagazine->Render();
    m_exBulletMagazine->Render();
    m_startAnimation->Render();

}

/// <summary> 終了処理 </summary>
void PlayScene::Finalize()
{
    GetColliders().clear();
    m_player.reset();

    for (auto& enemy: m_Enemy)
    {
        enemy.reset();
    }
    m_Enemy.clear();

    for (auto& dropItem : m_dropItem)
    {
        dropItem.reset();
    }
    m_dropItem.clear();

    for (auto& dropItem : m_dropItemB)
    {
        dropItem.reset();
    }
    m_dropItemB.clear();


    for (auto& hitParticle: m_hitParticle)
    {
        hitParticle.reset();
    }
    m_hitParticle.clear();
    
}

void PlayScene::UpdateBulletMagazine()
{
    m_bulletMagazine->Initialize(m_player->GetBulletSize());
    m_exBulletMagazine->Initialize(m_player->GetExBulletSize());
}

void PlayScene::SetNoise()
{
    m_postProcess->SetNoise(true);
}

/// <summary> ヒットエフェクトを生成する関数 </summary>
/// <param name="world"> ワールド行列 </param>
void PlayScene::CreateHitParticle(DirectX::SimpleMath::Matrix world)
{
    using namespace DirectX::SimpleMath;

    int particleValue = HitParticle::get_rand(1, 1);
    Vector3 pos = { world._41,world._42,world._43 };


    for (int i = 0; i < particleValue; i++)
    {

        float velocityX = (float)HitParticle::get_rand(-20, 20) / 500.0f;
        float velocityY = (float)HitParticle::get_rand(-20, 20) / 500.0f;
        float velocityZ = (float)HitParticle::get_rand(-20, 20) / 500.0f;

        m_hitParticle.emplace_back(std::make_unique<HitParticle>());
        m_hitParticle.back()->Initialize(pos, Vector3(velocityX, velocityY, velocityZ));
    }

   
}

void PlayScene::CreateHitParticle(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Quaternion rotate)
{
    using namespace DirectX::SimpleMath;

    int particleValue = HitParticle::get_rand(1, 5);
    Vector3 pos = { world._41,world._42,world._43 };

    for (int i = 0; i < particleValue; i++)
    {

        float velocityX = (float)HitParticle::get_rand(-20, 20) / 500.0f;
        float velocityY = (float)HitParticle::get_rand(-20, 20) / 500.0f;

        m_hitParticle.emplace_back(std::make_unique<HitParticle>());
        m_hitParticle.back()->Initialize(pos, Vector3::Transform({ velocityX,velocityY,0 }, rotate));
    }
}

void PlayScene::CreateHitEffect(DirectX::SimpleMath::Vector3 pos)
{
    m_hitEffect->Set(pos);
}

/// <summary>
/// 影用のテクスチャを作成
/// </summary>
void PlayScene::CreateShadow()
{
    Resources::GetInstance()->GetShadow()->BeginDepth();

    for (auto& wall : m_wall)
    {
        wall->CreateShadow();
    }

    for (auto& enemy : m_Enemy)
    {
        enemy->CreateShader();
    }

    for (auto& dropItem : m_dropItem)
    {
        dropItem->CreateShadow();
    }
    for (auto& dropItem : m_dropItemB)
    {
        dropItem->CreateShadow();
    }

    m_player->CreateShadow();

    Resources::GetInstance()->GetShadow()->EndDepth();
}

// Jsonから読み取った座標にオブジェクトを生成
void PlayScene::CreateObject(std::string className, DirectX::SimpleMath::Vector3 pos)
{
    using namespace DirectX::SimpleMath;
    if (className == "Player")
    {
         m_player = std::make_unique<Player>(this);
         m_player->Initialize();
         m_player->SetPosition(pos);
    }
    if (className == "BillA")
    {
        m_wall.emplace_back(std::make_unique<BillA>(this));
        m_wall.back()->SetPosition(pos);
        m_wall.back()->Initialize();
    }
    if (className == "BillB")
    {
        m_wall.emplace_back(std::make_unique<BillB>(this));
        m_wall.back()->SetPosition(pos);
        m_wall.back()->Initialize();
    }
    if (className == "HomingEnemy")
    {
        m_Enemy.emplace_back(std::make_unique<HomingEnemy>(this));
        m_Enemy.back()->SetPosition(pos);
        m_Enemy.back()->Initialize(m_player.get());
    }
    if (className == "FixedEnemy")
    {
        m_Enemy.emplace_back(std::make_unique<FixedEnemy>(this));
        m_Enemy.back()->SetPosition(pos);
        m_Enemy.back()->Initialize(m_player.get());
    }
    if (className == "BossEnemy")
    {
        m_Enemy.emplace_back(std::make_unique<BossEnemy>(this));
        m_Enemy.back()->SetPosition(pos);
        m_Enemy.back()->Initialize(m_player.get());
    }
    
}

std::vector<std::unique_ptr<Enemy>>::iterator PlayScene::RemoveEnemy(std::vector<std::unique_ptr<Enemy>>::iterator it)
{
    it->get()->Finalize();
    m_player->SetTarget(nullptr);
    return it = m_Enemy.erase(it);
}

void PlayScene::RemoveItem(std::vector<std::unique_ptr<DropItem>>::iterator it)
{
    Part::TypeID typeID = it->get()->GetPartType();
    m_player->SetPart(typeID, it->get()->GetPart());
    it = m_dropItem.erase(it);
}

void PlayScene::RemoveItemB(std::vector<std::unique_ptr<DropItemB>>::iterator it)
{
    it = m_dropItemB.erase(it);
}
