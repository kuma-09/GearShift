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
    m_player->SetPosition(Vector3(3, 0, 3));

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();
    m_enemy.back()->SetPosition(Vector3(0, 0, 0));

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();
    m_enemy.back()->SetPosition(Vector3(5, 0, 5));

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();
    m_enemy.back()->SetPosition(Vector3(10, 0, -5));

    m_enemyNum = 0;

    m_player->SetTarget(m_enemy[m_enemyNum]);

    m_wall.push_back(new Wall());
    m_wall.back()->SetPosition(Vector3(0, 0, 20));
    m_wall.back()->GetComponent<BoxCollider>().lock().get()->SetSize({ 50,10,1 });


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


    for (auto& enemy : m_enemy)
    {
        enemy->Update(elapsedTime);
    }

    for (auto& wall : m_wall)
    {
        wall->Update(elapsedTime);
    }

    for (auto& enemy : m_enemy)
    {
        CheckHit(*m_player->GetComponent<BoxCollider>().lock().get()->GetBoundingBox(), *enemy->GetComponent<BoxCollider>().lock().get()->GetBoundingBox());
    }

    for (auto& wall : m_wall)
    {
        CheckHit(*m_player->GetComponent<BoxCollider>().lock().get()->GetBoundingBox(), *wall->GetComponent<BoxCollider>().lock().get()->GetBoundingBox());
    }

    
    if (gp->a == gp->PRESSED)
    {
        m_enemyNum++;
        if (m_enemyNum >= m_enemy.size())
        {
            m_enemyNum = 0;
        }
        m_player->SetTarget(m_enemy[m_enemyNum]);
    }

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

void PlayScene::CheckHit(DirectX::BoundingBox a, DirectX::BoundingBox b)
{
    using namespace DirectX::SimpleMath;

    // ヒットしていなければ終わり
    if (!a.Intersects(b)) { return; }

    // 衝突時、ＢがＡを押し戻す処理========================

    // AABB用のmin/maxを計算する
    Vector3 aMin = a.Center - a.Extents;
    // ①aMax,bMin,bMaxを計算する

    // 各軸の差分を計算する
    float dx1 = (b.Center.x + b.Extents.x) - (a.Center.x - a.Extents.x);
    float dx2 = (b.Center.x - b.Extents.x) - (a.Center.x + a.Extents.x);
    // ②dy1,dy2,dz1,dz2を計算する
    float dy1 = (b.Center.y + b.Extents.y) - (a.Center.y - a.Extents.y);
    float dy2 = (b.Center.y - b.Extents.y) - (a.Center.y + a.Extents.y);

    float dz1 = (b.Center.z + b.Extents.z) - (a.Center.z - a.Extents.z);
    float dz2 = (b.Center.z - b.Extents.z) - (a.Center.z + a.Extents.z);

    // 各軸について、絶対値の小さい方を軸のめり込み量とする：AABBの重なった部分を特定する
    float dx = abs(dx1) < abs(dx2) ? dx1 : dx2;
    // ③dy,dzを計算する
    float dy = abs(dy1) < abs(dy2) ? dy1 : dy2;
    float dz = abs(dz1) < abs(dz2) ? dz1 : dz2;

    // 押し戻しベクトル
    Vector3 pushBackVec = Vector3::Zero;

    // めり込みが一番小さい軸を押し戻す（Ｘ軸が最小の場合）
    if (abs(dx) <= abs(dy) && abs(dx) <= abs(dz))
    {
        pushBackVec.x += dx;
    }
    // ④Ｙ軸、Ｚ軸が最小の場合を計算する
    if (abs(dy) <= abs(dx) && abs(dy) <= abs(dz))
    {
        pushBackVec.y += dy;
    }

    if (abs(dz) <= abs(dx) && abs(dz) <= abs(dy))
    {
        pushBackVec.z += dz;
    }

    // Ａを押し戻す（Ａの衝突判定情報も更新する）
    // ⑤
    m_player->SetPosition(m_player->GetPosition() + pushBackVec);
}