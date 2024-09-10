#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Game.h"
#include "Framework/Microsoft/DebugDraw.h"
#include "Game/Components/Camera.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"

#include "Game/Parts/Part.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/BodyBottom.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"

#include "Game/Particle/HitParticle.h"

PlayScene::PlayScene()
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

    m_timeLimit = 180.0f;
    m_totalTime = 0;

    // プレイヤー生成
    m_player = std::make_unique<Player>(this);
    m_player->SetPosition(Vector3(3, 5, 3));

    // パーツを装備
    m_player->SetPart(Part::Head, std::make_unique<Head>());
    m_player->SetPart(Part::BodyTop,std::make_unique<BodyTop>());
    m_player->SetPart(Part::BodyBottom, std::make_unique<BodyBottom>());
    m_player->SetPart(Part::LeftArm, std::make_unique<LeftArm>());
    m_player->SetPart(Part::RightArm, std::make_unique<RightArm>());
    m_player->SetPart(Part::LeftLeg,  std::make_unique<LeftLeg>());
    m_player->SetPart(Part::RightLeg, std::make_unique<RightLeg>());

    m_player->Initialize();

    m_enemy.push_back(std::make_unique<Enemy>(this));
    m_enemy.back()->Initialize(m_player.get());
    m_enemy.back()->SetPosition(Vector3(0, 0, 0));

    m_enemy.push_back(std::make_unique<Enemy>(this));
    m_enemy.back()->Initialize(m_player.get());
    m_enemy.back()->SetPosition(Vector3(2, 0, 3));

    m_enemy.push_back(std::make_unique<Enemy>(this));
    m_enemy.back()->Initialize(m_player.get());
    m_enemy.back()->SetPosition(Vector3(-4, 0, -5));

    m_wall.push_back(std::make_unique<Wall>(this));
    m_wall.back()->SetScale({ 10, 10, 10 });
    m_wall.back()->SetPosition({ 5, 5, 0 });


    m_dropItem.push_back(std::make_unique<DropItem>(this, std::make_unique<BodyTop>()));
 
    m_dropItem.back()->SetPosition(Vector3(0, 0, 7));
    m_dropItem.back()->Initialize();

    m_floor = std::make_unique<Floor>(this);
    m_skyDome = std::make_unique<SkyDome>();

    m_enemyNum = 0;
    m_player->SetTarget(m_enemy[m_enemyNum].get());

    m_debugString = std::make_unique<DebugString>(
          m_deviceResources->GetD3DDevice()
        , m_deviceResources->GetD3DDeviceContext()
        , L"Resources/Fonts/SegoeUI_18.spritefont");
    m_debugString->SetColor(DirectX::Colors::Red);




}

/// <summary> 更新処理 </summary>
/// <param name="elapsedTime"> 経過時間 </param>
void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    
    const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    m_totalTime += elapsedTime;
    m_debugString->AddString(std::to_string(m_timeLimit - m_totalTime).c_str());

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

    m_skyDome->Update(elapsedTime);
    m_floor->Update(elapsedTime);
    m_player->Update(elapsedTime);

    for (auto& enemy : m_enemy)
    {
        enemy->Update(elapsedTime);

    }
    
    for (auto& dropItem : m_dropItem)
    {
        dropItem->Update(elapsedTime);
    }

    for (auto& wall : m_wall)
    {
        wall->Update(elapsedTime);
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
            if (kb->pressed.X)
            {
                Part::TypeID typeID = it->get()->GetPartType();
                m_player->SetPart(typeID, it->get()->GetPart());
                RemoveCollider(it->get()->GetComponent<BoxCollider>());
                it = m_dropItem.erase(it);
                break;
            }
        }
        else it->get()->SetHit(false);
    }

    BoxCollider::CheckHit(m_player.get(), m_floor.get());
    BoxCollider::CheckHit(m_player.get(), m_wall.back().get());

    // 体力の無い敵を削除
    for (auto it = m_enemy.begin(); it != m_enemy.end();)
    {
        BoxCollider::CheckHit(m_player.get(), it->get());
        BoxCollider::CheckHit(it->get(), m_floor.get());
        if (it->get()->GetHP() > 0)
        {
            it++;
        }
        else
        {
            RemoveCollider(it->get()->GetComponent<BoxCollider>());
            it->get()->Finalize();
            it = m_enemy.erase(it);
            NextTarget();
            break;
        }
    }
    
    // ターゲット切り替え
    if (gp->a == gp->PRESSED || kb->IsKeyPressed(DirectX::Keyboard::Z))
    {
        NextTarget();
    }


}

/// <summary> 描画処理 </summary>
void PlayScene::Render()
{
    using namespace DirectX::SimpleMath;

    m_skyDome->Render();

    m_player->Render();

    m_floor->Render();

    for (auto& enemy: m_enemy)
    {
        enemy->Render();
    }

    m_enemy[m_enemyNum].get()->GetComponent<HPBar>()->Render(m_enemy[m_enemyNum]->GetPosition());

    for (auto& dropItem : m_dropItem)
    {
        dropItem->Render();
    }

    for (auto& wall : m_wall)
    {
        wall->Render();
    }

    auto state = m_graphics->GetCommonStates();

    for (auto& particle : m_hitParticle)
    {
        particle->Render(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    }

    m_debugString->Render(state);

    
}

/// <summary> 終了処理 </summary>
void PlayScene::Finalize()
{
    m_pBoxCollider.clear();
    m_player.reset();

    for (auto& enemy: m_enemy)
    {
        enemy.reset();
    }
    m_enemy.clear();

    for (auto& dropItem : m_dropItem)
    {
        dropItem.reset();
    }
    m_dropItem.clear();

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
        float velocityZ = (float)HitParticle::get_rand(-10, 10) / 100.0f;

        m_hitParticle.push_back(std::make_unique<HitParticle>());
        m_hitParticle.back()->Initialize(pos, Vector3(velocityX, velocityY, velocityZ));
    }


}

/// <summary> ターゲットを切り替える関数 </summary>
void PlayScene::NextTarget()
{
    m_enemyNum++;
    if (m_enemyNum >= m_enemy.size())
    {
        m_enemyNum = 0;
    }

    if (m_enemy.empty())
    {
        GetGame()->ChangeScene(GetGame()->GetResultScene());
        return;
    }
    m_player->SetTarget(m_enemy[m_enemyNum].get());
}