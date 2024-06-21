#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Framework/Microsoft/DebugDraw.h"
#include "Game/Components/Camera.h"
#include "Game/Components/BoxCollider.h"



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

    m_enemyNum = 0;

    m_player->SetTarget(m_enemy[m_enemyNum].get());

    m_skyDome = std::make_unique<SkyDome>();

    m_wall.push_back(std::make_unique<Wall>(this));
    m_wall.back()->SetPosition(Vector3(0, 0, 20));
    m_wall.back()->GetComponent<BoxCollider>().lock().get()->SetSize({ 50,10,1 });

    m_dropItem.push_back(std::make_unique<DropItem>(this));
    m_dropItem.back()->SetPosition(Vector3(3, 0, 7));


    // 四角形の頂点座標を定義する…左下基準のコの字、頂点順の指定でDrawQuadが使える
    m_vertices[0] = { Vector3(-50.0f ,-0.5f, 50.0f),Vector4(0,0.8f,0,1), Vector2(0.0f, 0.0f) };	//左上
    m_vertices[1] = { Vector3( 50.0f ,-0.5f, 50.0f),Vector4(0,0.8f,0,1), Vector2(1.0f, 0.0f)};	//右上
    m_vertices[2] = { Vector3(-50.0f ,-0.5f,-50.0f),Vector4(0,0.8f,0,1), Vector2(0.0f, 1.0f) };	//左下
    m_vertices[3] = { Vector3( 50.0f ,-0.5f,-50.0f),Vector4(0,0.8f,0,1), Vector2(1.0f, 1.0f) };	//右下


    DirectX::CreateWICTextureFromFile(
        m_deviceResources->GetD3DDevice(),		// デバイスコンテキスト
        L"Resources/Textures/white.png",	    // 画像ファイルのパス
        nullptr,								// 内部的なテクスチャ
        m_texture.ReleaseAndGetAddressOf()		// シェーダリソースビュー(表示用)
    );

    
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
                    int a = 0;
                    break;
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
            int i = 111;
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

    //m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    //m_graphics->GetPrimitiveBatch()->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
    //m_graphics->DrawPrimitiveEnd();

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
