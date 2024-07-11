#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Game.h"
#include "Framework/Microsoft/DebugDraw.h"
#include "Game/Components/Camera.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"

#include "Game/Parts/IPart.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/BodyBottom.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"

PlayScene::PlayScene()
{

}

void PlayScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    SetGame(game);

    m_player = std::make_unique<Player>(this);
    m_player->SetPosition(Vector3(3, 10, 3));

    
    m_player->SetPart("Head", std::make_unique<Head>());
    m_player->SetPart("BodyTop",std::make_unique<BodyTop>());
    m_player->SetPart("BodyBottom", std::make_unique<BodyBottom>());
    m_player->SetPart("LeftArm", std::make_unique<LeftArm>());
    m_player->SetPart("RightArm", std::make_unique<RightArm>());
    m_player->SetPart("LeftLeg",  std::make_unique<LeftLeg>());
    m_player->SetPart("RightLeg", std::make_unique<RightLeg>());

    m_player->Initialize();


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

    m_dropItem.push_back(std::make_unique<DropItem>(this,std::make_unique<LeftLeg>()));
    m_dropItem.back()->SetPosition(Vector3(3, 0, 7));

    m_skyDome = std::make_unique<SkyDome>();

    m_enemyNum = 0;
    m_player->SetTarget(m_enemy[m_enemyNum].get());

    m_debugString = std::make_unique<DebugString>(
          m_deviceResources->GetD3DDevice()
        , m_deviceResources->GetD3DDeviceContext()
        , L"Resources/Fonts/SegoeUI_18.spritefont");
    m_debugString->SetColor(DirectX::Colors::Red);
}


void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    
    const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    m_skyDome->Update(elapsedTime);

    m_player->Update(elapsedTime);

    for (auto it = m_enemy.begin(); it != m_enemy.end(); it++)
    {
        
        it->get()->Update(elapsedTime);
        m_debugString->AddString(std::to_string(it->get()->GetHP()).c_str());
        BoxCollider::CheckHit(m_player.get(), it->get());
        if (it->get()->GetHP() <= 0)
        {
            RemoveCollider(it->get()->GetComponent<BoxCollider>().lock().get());
            m_enemy.erase(it);
            NextTarget();
            break;
        }
    }
    

    for (auto& dropItem : m_dropItem)
    {
        dropItem->Update(elapsedTime);
    }

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
    

    for (auto it = m_dropItem.begin(); it != m_dropItem.end(); it++)
    {
        if (m_player->GetComponent<BoxCollider>().lock().get()->
            GetBoundingBox()->Intersects(
                *it->get()->GetComponent<BoxCollider>().lock().get()->GetBoundingBox()))
        {
            it->get()->SetHit(true);
            if (kb->pressed.X)
            {
                m_player->SetPart("LeftLeg", it->get()->GetPart());
                m_dropItem.erase(it);
                break;
            }
        }
        else it->get()->SetHit(false);
    }

    if (gp->a == gp->PRESSED || kb->IsKeyPressed(DirectX::Keyboard::Z))
    {
        NextTarget();
    }


}

void PlayScene::Render()
{
    using namespace DirectX::SimpleMath;

    m_skyDome->Render();

    m_player->Render();

    for (auto& enemy: m_enemy)
    {
        enemy->Render();
    }

    m_enemy[m_enemyNum].get()->GetComponent<HPBar>().lock().get()->Render(m_enemy[m_enemyNum]->GetPosition());

    for (auto& wall: m_wall)
    {
        wall->Render();
    }

    for (auto& dropItem : m_dropItem)
    {
        dropItem->Render();
    }

    auto state = m_graphics->GetCommonStates();

    m_debugString->Render(state);
}

void PlayScene::Finalize()
{
    ClearColliders();
    m_player->Finalize();
}

void PlayScene::NextTarget()
{
    m_enemyNum++;
    if (m_enemyNum >= m_enemy.size())
    {
        m_enemyNum = 0;
    }

    if (m_enemy.empty())
    {
        GetGame()->ChangeScene(GetGame()->GetTitleScene());
        //m_player->SetTarget(m_player.get());
        return;
    }
    m_player->SetTarget(m_enemy[m_enemyNum].get());
}