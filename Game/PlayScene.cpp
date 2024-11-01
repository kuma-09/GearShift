#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Game.h"

#include "Framework/Microsoft/DebugDraw.h"
#include "Game/Components/HP.h"
#include "Game/Components/Camera.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"

#include "Game/Parts/Part.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"

#include "Game/Object/Wall/BillA.h"
#include "Game/Object/Wall/BillB.h"
#include "Game/Object/Bullet/HomingBullet.h"

#include "Game/Particle/HitParticle.h"

#include "UI/HPUI.h"

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

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    SetGame(game);

    m_timeLimit = 180.0f;
    m_totalTime = 0;

    // プレイヤー生成
    m_player = std::make_unique<Player>(this);
    m_player->SetPosition(Vector3(3, 5, 10));

    // パーツを装備
    m_player->SetPart(Part::Head, std::make_unique<Head>());
    m_player->SetPart(Part::BodyTop,std::make_unique<BodyTop>());
    m_player->SetPart(Part::LeftArm, std::make_unique<LeftArm>());
    m_player->SetPart(Part::RightArm, std::make_unique<RightArm>());
    m_player->SetPart(Part::LeftLeg,  std::make_unique<LeftLeg>());
    m_player->SetPart(Part::RightLeg, std::make_unique<RightLeg>());

    m_player->Initialize();

    m_Enemy.emplace_back(std::make_unique<HomingEnemy>(this));
    m_Enemy.back()->Initialize(m_player.get());
    m_Enemy.back()->SetPosition(Vector3(0, 5, 0));

    m_Enemy.emplace_back(std::make_unique<HomingEnemy>(this));
    m_Enemy.back()->Initialize(m_player.get());
    m_Enemy.back()->SetPosition(Vector3(32, 5, 10));

    m_Enemy.emplace_back(std::make_unique<FixedEnemy>(this));
    m_Enemy.back()->Initialize(m_player.get());
    m_Enemy.back()->SetPosition(Vector3(50, 5, 10));

    m_wall.emplace_back(std::make_unique<BillA>(this));
    m_wall.back()->SetPosition({5,9,40});
    m_wall.back()->Initialize();

    m_wall.emplace_back(std::make_unique<BillB>(this));
    m_wall.back()->SetPosition({ -6,9,-20 });
    m_wall.back()->Initialize();

    m_wall.emplace_back(std::make_unique<BillA>(this));
    m_wall.back()->SetPosition({ 30,9,40 });
    m_wall.back()->Initialize();

    m_wall.emplace_back(std::make_unique<BillB>(this));
    m_wall.back()->SetPosition({ -14,9,-10 });
    m_wall.back()->Initialize();


    m_dropItem.emplace_back(std::make_unique<DropItem>(this, std::make_unique<BodyTop>()));
    m_dropItem.back()->SetPosition(Vector3(0, 20, 20));

    //m_dropItem.emplace_back(std::make_unique<DropItem>(this, std::make_unique<LeftArm>()));
    //m_dropItem.back()->SetPosition(Vector3(6, 3, 9));

    std::vector<std::unique_ptr<Bullet>> bullets;
    for (int i = 0; i < 10; i++)
    {
        bullets.emplace_back(std::make_unique<HomingBullet>(this, BoxCollider::PlayerBullet));
    }

    m_dropItemB.emplace_back(std::make_unique<DropItemB>(this, std::move(bullets)));
    m_dropItemB.back()->SetPosition(Vector3(10, 3, 20));

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

}

/// <summary> 更新処理 </summary>
/// <param name="elapsedTime"> 経過時間 </param>
void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    // ゲームパッドとキーボードの入力情報を取得
    //const auto& gp = m_inputManager->GetGamePadTracker();
    //const auto& kb = m_inputManager->GetKeyboardTracker();

    // 経過時間を計算
    m_totalTime += elapsedTime;
    m_postProcess->Update(elapsedTime);
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

    std::vector<Enemy*> inAreaEnemy;

    // 体力の無い敵を削除
    for (auto it = m_Enemy.begin(); it != m_Enemy.end();)
    {
        it->get()->Update(elapsedTime);
        if (m_targetArea->Update(m_player.get(), it->get()))
        {

            Vector3 dir = it->get()->GetPosition() - m_player->GetPosition();
            dir.Normalize();
            Ray ray = { m_player->GetPosition(),dir };
            float n = 0;
            
            int i = 0;
            for (auto& wall : m_wall)
            {
                if (ray.Intersects(*wall->GetComponent<BoxCollider>()->GetBoundingBox(), n))
                {
                    if (n <= (it->get()->GetPosition() - m_player->GetPosition()).Length())
                    {
                        i++;
                    }
                }
            }

            if (i == 0 && it->get()->GetComponent<HP>()->GetHP() > 0)
            {
                inAreaEnemy.emplace_back(it->get());
            }
        }
        if (it->get()->GetComponent<HP>()->GetHP() > 0)
        {
            it++;
        }
        else
        {
            
            it = RemoveEnemy(it);
            if (m_Enemy.empty())
            {
                GetGame()->ChangeScene(GetGame()->GetResultScene());
                return;
            }
        }
    }

    if (inAreaEnemy.empty())
    {
        m_player->SetTarget(nullptr);
    }
    else
    {
        for (auto enemy : inAreaEnemy)
        {
            if (enemy->GetComponent<HP>()->GetHP() > 0)
            {
                m_player->SetTarget(enemy);
                m_postProcess->SetNoise(true);
                break;
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

            m_player->AddWepon(it->get()->GetPart());
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
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    CreateShadow();

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
    m_player->Render();
    for (auto& enemy : m_Enemy)
    {
        enemy->Render();
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
        m_player->GetTarget()->GetComponent<ModelDraw>()->OutLineRender();
    }
    for (auto& particle : m_hitParticle)
    {
        particle->Render(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    }
    m_hitEffect->Render();

    // UI
    m_targetArea->Render(m_player->GetTarget());
    m_hpUI->Render();

    // Bloom-------------------------------------
    m_postProcess->BeginBloom();
    for (auto& particle : m_hitParticle)
    {
        particle->Render(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    }

    m_targetArea->Render(m_player->GetTarget());
    // リソースの解除＆ライトをキューブで描画
    Resources::GetInstance()->GetShadow()->End();

    //-------------------------------------------

    m_postProcess->combinationRT();

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

/// <summary> ヒットエフェクトを生成する関数 </summary>
/// <param name="world"> ワールド行列 </param>
void PlayScene::CreateHitParticle(DirectX::SimpleMath::Matrix world)
{
    using namespace DirectX::SimpleMath;



    int particleValue = HitParticle::get_rand(5, 10);
    Vector3 pos = { world._41,world._42,world._43 };


    for (int i = 0; i < particleValue; i++)
    {

        float velocityX = (float)HitParticle::get_rand(-10, 10) / 100.0f;
        float velocityY = (float)HitParticle::get_rand(-10, 10) / 100.0f;
        float velocityZ = (float)HitParticle::get_rand(5, 20) / 100.0f;

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

        float velocityX = (float)HitParticle::get_rand(-10, 10) / 1000.0f;
        float velocityY = (float)HitParticle::get_rand(-10, 10) / 1000.0f;
        float velocityZ = (float)HitParticle::get_rand(-30, 30) / 1000.0f;

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

std::vector<std::unique_ptr<Enemy>>::iterator PlayScene::RemoveEnemy(std::vector<std::unique_ptr<Enemy>>::iterator it)
{
    //RemoveCollider(it->get()->GetComponent<BoxCollider>());
    it->get()->Finalize();
    m_player->SetTarget(nullptr);
    return it = m_Enemy.erase(it);
}

void PlayScene::RemoveItem(std::vector<std::unique_ptr<DropItem>>::iterator it)
{
    Part::TypeID typeID = it->get()->GetPartType();
    m_player->SetPart(typeID, it->get()->GetPart());
    //RemoveCollider(it->get()->GetComponent<BoxCollider>());
    it = m_dropItem.erase(it);
}

void PlayScene::RemoveItemB(std::vector<std::unique_ptr<DropItemB>>::iterator it)
{
    //m_player->AddWepon(it);
    //RemoveCollider(it->get()->GetComponent<BoxCollider>());
    it = m_dropItemB.erase(it);
}
