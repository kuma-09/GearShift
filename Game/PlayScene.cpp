#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Framework/Microsoft/DebugDraw.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Game/Camera.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Object/Wall.h"

void PlayScene::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    m_player = new Player();
    m_player->Initialize();

    m_camera = new Camera();
    m_camera->Initialize();

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();
    m_enemy.back()->SetPosition(Vector3(0, 0, 0));

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();
    m_enemy.back()->SetPosition(Vector3(5, 0, 5));

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();
    m_enemy.back()->SetPosition(Vector3(10, 0, -5));

    //m_player->SetTarget(m_enemy.front());

    m_enemyNum = 0;

    m_wall.push_back(new Wall());
    m_wall.back()->SetPosition(Vector3(0, 0, 10));


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

    UNREFERENCED_PARAMETER(gp);

    m_player->Update(elapsedTime);
    m_camera->Update(elapsedTime, m_player, m_enemy.front());


    for (auto& enemy : m_enemy)
    {
        enemy->Update(elapsedTime);
    }

    for (auto& wall : m_wall)
    {
        wall->Update(elapsedTime);
        if (m_player->GetComponent<BoxCollider>().lock().get()->GetBoundingBox()->Intersects(*wall->GetComponent<BoxCollider>().lock().get()->GetBoundingBox()))
        {
            int i = 1;
        }
    }


    
    //if (gp->a == gp->PRESSED)
    //{
    //    m_enemyNum++;
    //    if (m_enemyNum >= m_enemy.size())
    //    {
    //        m_enemyNum = 0;
    //    }
    //    m_player->SetTarget(m_enemy.at(m_enemyNum));
    //}

}

void PlayScene::Render()
{
    using namespace DirectX::SimpleMath;

    m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());

    m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    //m_graphics->GetPrimitiveBatch()->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
    m_graphics->DrawPrimitiveEnd();

    m_player->Render();

    for (auto& enemy: m_enemy)
    {
        enemy->Render();
    }

    for (auto& wall: m_wall)
    {
        wall->Render();
    }


}

void PlayScene::Finalize()
{
    m_player->Finalize();
    delete m_player;
    m_player = nullptr;
    delete m_graphics;
    m_graphics = nullptr;
    delete m_deviceResources;
    m_deviceResources = nullptr;
    delete m_inputManager;
    m_inputManager = nullptr;
}