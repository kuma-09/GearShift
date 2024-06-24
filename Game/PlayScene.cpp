#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Framework/Microsoft/DebugDraw.h"
#include "Game/Components/Camera.h"
#include "Game/Components/BoxCollider.h"

#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/BodyBottom.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"


void PlayScene::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    m_player = std::make_unique<Player>(this);
    m_player->Initialize();
    m_player->SetPosition(Vector3(3, 10, 3));

    m_enemy.push_back(std::make_unique<Enemy>(this));
    m_enemy.back()->Initialize(m_player.get());
    m_enemy.back()->SetPosition(Vector3(0, 0, 0));

    m_enemy.push_back(std::make_unique<Enemy>(this));
    m_enemy.back()->Initialize(m_player.get());
    m_enemy.back()->SetPosition(Vector3(5, 0, 5));

    m_enemy.push_back(std::make_unique<Enemy>(this));
    m_enemy.back()->Initialize(m_player.get());
    m_enemy.back()->SetPosition(Vector3(10, 0, -5));

    m_wall.push_back(std::make_unique<Wall>(this));
    m_wall.back()->SetPosition(Vector3(0, 0, 20));
    m_wall.back()->GetComponent<BoxCollider>().lock().get()->SetSize({ 50,10,1 });

    m_dropItem.push_back(std::make_unique<DropItem>(this));
    m_dropItem.back()->SetPosition(Vector3(3, 0, 7));

    m_skyDome = std::make_unique<SkyDome>();

    m_player->SetTarget(m_enemy[0].get());

}


void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    
    const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    m_skyDome->Update(elapsedTime);

    m_player->Update(elapsedTime);

    for (auto& enemy : m_enemy)
    {
        enemy->Update(elapsedTime);
    }

    for (auto& wall : m_wall)
    {
        wall->Update(elapsedTime);
    }

    for (auto& dropItem : m_dropItem)
    {
        dropItem->Update(elapsedTime);
    }

    for (auto& enemy : m_enemy)
    {
         BoxCollider::CheckHit(m_player.get(), enemy.get());
    }

    for (auto& wall : m_wall)
    {
         BoxCollider::CheckHit(m_player.get(), wall.get());
    }


    for (auto& collider: GetColliders())
    {
        if (collider->GetTypeID() == BoxCollider::TypeID::EnemyBullet)
        {
            for (auto& playerCollider: GetColliders())
            {
                if (playerCollider->GetTypeID() == BoxCollider::TypeID::Player &&
                    collider->GetBoundingBox()->Intersects(*playerCollider->GetBoundingBox()))
                {
                    playerCollider->GetOwner()->Damage(1);
                }
            }
        }
    }

    for (auto& dropItem : m_dropItem)
    {
        if (m_player->GetComponent<BoxCollider>().lock().get()->
            GetBoundingBox()->Intersects(
                *dropItem.get()->GetComponent<BoxCollider>().lock().get()->GetBoundingBox()))
        {

        }
    }

    
    if (gp->a == gp->PRESSED || kb->IsKeyPressed(DirectX::Keyboard::Z))
    {
        m_enemyNum++;
        if (m_enemyNum >= m_enemy.size())
        {
            m_enemyNum = 0;
        }
        m_player->SetTarget(m_enemy[m_enemyNum].get());
    }
}

void PlayScene::Render()
{
    using namespace DirectX::SimpleMath;

    m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());

    m_skyDome->Render();

    m_player->Render();

    for (auto& enemy: m_enemy)
    {
        enemy->Render();
    }

    for (auto& wall: m_wall)
    {
        wall->Render();
    }

    for (auto& dropItem : m_dropItem)
    {
        dropItem->Render();
    }

}

void PlayScene::Finalize()
{

}
