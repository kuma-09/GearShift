#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Game.h"

#include "Framework/Microsoft/DebugDraw.h"
#include "Framework/Audio.h"
#include "Framework/Json.h"

#include "Player/Player.h"
#include "Enemy/FixedEnemy.h"
#include "Enemy/HomingEnemy.h"
#include "Enemy/BossEnemy.h"
#include "Game/Object/Wall/BillA.h"
#include "Game/Object/Wall/BillB.h"
#include "Game/Object/Bullet/HomingBullet.h"

#include "Game/Particle/HitParticle.h"

#include "Game/Animation/StartAnimation.h"
#include "UI/BulletMagazine.h"
#include "UI/ExBulletMagazine.h"

#include "System/HitStop.h"

#include "Manager/RenderManager.h"
#include "Manager/ObjectManager.h"
#include "Manager/CollisionManager.h"

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
    //Finalize();
}

/// <summary> ���������� </summary>
/// <param name="game"> �Q�[�� </param>
void PlayScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    SetGame(game);

    // ��������
    m_timeLimit = 180.0f;
    m_totalTime = 0;

    // Json�t�@�C����Data��ǂݍ���
    std::vector<std::string> str;
    std::vector<Vector3> pos;
    Json::LoadJsonFile(L"Stage1.json", str, pos);

    // �I�u�W�F�N�g�̐���
    for (int i = 0; i < str.size(); i++)
    {
        CreateObject(str[i], pos[i]);
    }

    m_floor.emplace_back(std::make_unique<Floor>(this));

    m_skyDome = std::make_unique<SkyDome>();
    m_skyDome->Initialize(Vector3::Zero);

    m_targetArea = std::make_unique<TargetArea>();
    m_targetArea->Initialize();

    m_postProcess = std::make_unique<PostProcess>();
    m_postProcess->Initialize();

    m_hitEffect = std::make_unique<ExplosionEffect>();
    m_hitEffect->Initialize();

    m_startAnimation = std::make_unique<StartAnimation>();
    m_startAnimation->Initialize();
    m_bulletMagazine = std::make_unique<BulletMagazine>();
    m_bulletMagazine->Initialize(10);
    m_exBulletMagazine = std::make_unique<ExBulletMagazine>();
    m_exBulletMagazine->Initialize(0);
    
}

/// <summary> �X�V���� </summary>
/// <param name="elapsedTime"> �o�ߎ��� </param>
void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    HitStop::Update(elapsedTime);
    if (HitStop::GetIsStop()) return;

    // �o�ߎ��Ԃ��v�Z
    m_totalTime += elapsedTime;
    m_postProcess->Update(elapsedTime);
    m_startAnimation->Update(elapsedTime);
    m_targetArea->ClearTarget();

    // �q�b�g�G�t�F�N�g�̍X�V
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

    // �I�u�W�F�N�g�̍X�V
    auto enemys = ObjectManager::GetTypeObjects(Type::Enemy);
    if (enemys.empty())
    {
        GetGame()->ChangeScene(GetGame()->GetResultScene());
    }

    for (auto& enemy : enemys)
    {
        m_targetArea->Update(m_player.lock().get(), enemy.lock().get(),static_cast<Player*>(m_player.lock().get())->GetCamera());
    }

    static_cast<Player*>(m_player.lock().get())->SetTarget(m_targetArea->GetTarget());

    ObjectManager::Update(elapsedTime);
    for (auto& floor : m_floor)
    {
        floor->Update(elapsedTime);
    }

    CollisionManager::Update();

    ObjectManager::Delete();
}

/// <summary> �`�揈�� </summary>
void PlayScene::Render()
{
    m_skyDome->Render();
    RenderManager::RenderObjects();
}

void PlayScene::TranslucentRender()
{
    RenderManager::RenderParticle();
    for (auto& particle : m_hitParticle)
    {
        particle->Render(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    }
    //if (m_player->GetTarget())
    //{
    //    m_player->GetTarget()->GetComponent<HPBar>()->Render(m_player->GetTarget()->GetPosition());
    //}
    m_hitEffect->Render();
}

void PlayScene::RenderUI()
{
    // UI
    m_targetArea->Render(m_targetArea->GetTarget());
    static_cast<Player*>(m_player.lock().get())->RenderPlayerUI();
    //m_bulletMagazine->Render();
    //m_exBulletMagazine->Render();
    m_startAnimation->Render();
}

/// <summary> �I������ </summary>
void PlayScene::Finalize()
{
    ObjectManager::Clear();
    RenderManager::Clear();
}



void PlayScene::SetNoise()
{
    m_postProcess->SetNoise(true);
}

/// <summary> �q�b�g�G�t�F�N�g�𐶐�����֐� </summary>
/// <param name="world"> ���[���h�s�� </param>
void PlayScene::CreateHitParticle(DirectX::SimpleMath::Matrix world)
{
    using namespace DirectX::SimpleMath;

    int particleValue = HitParticle::get_rand(5, 10);
    Vector3 pos = { world._41,world._42,world._43 };


    for (int i = 0; i < particleValue; i++)
    {

        float velocityX = (float)HitParticle::get_rand(-30, 30) / 500.0f;
        float velocityY = (float)HitParticle::get_rand(-30, 30) / 500.0f;
        float velocityZ = (float)HitParticle::get_rand(-30, 30) / 500.0f;

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

// Json����ǂݎ�������W�ɃI�u�W�F�N�g�𐶐�
void PlayScene::CreateObject(std::string className, DirectX::SimpleMath::Vector3 pos)
{
    using namespace DirectX::SimpleMath;
    if (className == "Player")
    {
        m_player = ObjectManager::Add(std::make_shared<Player>(this),pos);
    }
    if (className == "BillA")
    {
        ObjectManager::Add(std::make_shared<BillA>(this),pos);
    }
    if (className == "BillB")
    {
        ObjectManager::Add(std::make_shared<BillB>(this),pos);
    }
    if (className == "HomingEnemy")
    {
        ObjectManager::Add(std::make_shared<HomingEnemy>(this,m_player.lock().get()),pos, Type::Enemy);
    }
    if (className == "FixedEnemy")
    {
        ObjectManager::Add(std::make_shared<FixedEnemy>(this, m_player.lock().get()),pos, Type::Enemy);
    }
    if (className == "BossEnemy")
    {
        ObjectManager::Add(std::make_shared<BossEnemy>(this, m_player.lock().get()),pos, Type::Enemy);
    }
    if (className == "DropItem")
    {
        ObjectManager::Add(std::make_shared<DropItem>(this), pos);
    }
    if (className == "DropItemB")
    {
        ObjectManager::Add(std::make_shared<DropItemB>(this),pos);
    }   
}