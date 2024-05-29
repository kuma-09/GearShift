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


    // �l�p�`�̒��_���W���`����c������̃R�̎��A���_���̎w���DrawQuad���g����
    m_vertices[0] = { Vector3(-50.0f ,-0.5f, 50.0f),Vector4(0,0.8f,0,1), Vector2(0.0f, 0.0f) };	//����
    m_vertices[1] = { Vector3( 50.0f ,-0.5f, 50.0f),Vector4(0,0.8f,0,1), Vector2(1.0f, 0.0f)};	//�E��
    m_vertices[2] = { Vector3(-50.0f ,-0.5f,-50.0f),Vector4(0,0.8f,0,1), Vector2(0.0f, 1.0f) };	//����
    m_vertices[3] = { Vector3( 50.0f ,-0.5f,-50.0f),Vector4(0,0.8f,0,1), Vector2(1.0f, 1.0f) };	//�E��


    DirectX::CreateWICTextureFromFile(
        m_deviceResources->GetD3DDevice(),		// �f�o�C�X�R���e�L�X�g
        L"Resources/Textures/white.png",	    // �摜�t�@�C���̃p�X
        nullptr,								// �����I�ȃe�N�X�`��
        m_texture.ReleaseAndGetAddressOf()		// �V�F�[�_���\�[�X�r���[(�\���p)
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

    // �q�b�g���Ă��Ȃ���ΏI���
    if (!a.Intersects(b)) { return; }

    // �Փˎ��A�a���`�������߂�����========================

    // AABB�p��min/max���v�Z����
    Vector3 aMin = a.Center - a.Extents;
    // �@aMax,bMin,bMax���v�Z����

    // �e���̍������v�Z����
    float dx1 = (b.Center.x + b.Extents.x) - (a.Center.x - a.Extents.x);
    float dx2 = (b.Center.x - b.Extents.x) - (a.Center.x + a.Extents.x);
    // �Ady1,dy2,dz1,dz2���v�Z����
    float dy1 = (b.Center.y + b.Extents.y) - (a.Center.y - a.Extents.y);
    float dy2 = (b.Center.y - b.Extents.y) - (a.Center.y + a.Extents.y);

    float dz1 = (b.Center.z + b.Extents.z) - (a.Center.z - a.Extents.z);
    float dz2 = (b.Center.z - b.Extents.z) - (a.Center.z + a.Extents.z);

    // �e���ɂ��āA��Βl�̏������������̂߂荞�ݗʂƂ���FAABB�̏d�Ȃ�����������肷��
    float dx = abs(dx1) < abs(dx2) ? dx1 : dx2;
    // �Bdy,dz���v�Z����
    float dy = abs(dy1) < abs(dy2) ? dy1 : dy2;
    float dz = abs(dz1) < abs(dz2) ? dz1 : dz2;

    // �����߂��x�N�g��
    Vector3 pushBackVec = Vector3::Zero;

    // �߂荞�݂���ԏ��������������߂��i�w�����ŏ��̏ꍇ�j
    if (abs(dx) <= abs(dy) && abs(dx) <= abs(dz))
    {
        pushBackVec.x += dx;
    }
    // �C�x���A�y�����ŏ��̏ꍇ���v�Z����
    if (abs(dy) <= abs(dx) && abs(dy) <= abs(dz))
    {
        pushBackVec.y += dy;
    }

    if (abs(dz) <= abs(dx) && abs(dz) <= abs(dy))
    {
        pushBackVec.z += dz;
    }

    // �`�������߂��i�`�̏Փ˔�������X�V����j
    // �D
    m_player->SetPosition(m_player->GetPosition() + pushBackVec);
}