#include "pch.h"
#include "PlayScene.h"
#include "cmath"
#include "Framework/Microsoft/DebugDraw.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"

void PlayScene::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();



    m_basicEffect = m_graphics->GetBasicEffect();
    m_primitiveBatch = m_graphics->GetPrimitiveBatch();

    // �r���[�s����쐬
    Vector3 eye = Vector3(0, 5, 10);
    Vector3 target = Vector3::Zero;
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);

    // �ˉe�s����쐬����
    Matrix projection = Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(45.0f),
        static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().right) / static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().bottom),
        0.1f, 100.0f
    );
    m_graphics->SetProjectionMatrix(projection);


    m_player = new Player();
    m_player->Initialize();

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();
    m_enemy.back()->SetPosition(Vector3(5, 0, 5));

    m_enemy.push_back(new Enemy());
    m_enemy.back()->Initialize();
    m_enemy.back()->SetPosition(Vector3(10, 0, -5));
    
    m_player->SetTarget(m_enemy.front());

    m_enemyNum = 0;

    m_enemyBox = std::make_unique<BoundingBox>();
    m_enemyBox->Extents = Vector3(0.5f, 0.5f, 0.5f);

    // �l�p�`�̒��_���W���`����c������̃R�̎��A���_���̎w���DrawQuad���g����
    m_vertices[0] = { Vector3(-5.0f ,5.0f , 5.0f),Vector4(1,1,1,1), Vector2(0.0f, 0.0f) };	//����
    m_vertices[1] = { Vector3( 5.0f ,5.0f , 5.0f),Vector4(1,1,1,1), Vector2(1.0f, 0.0f) };	    //�E��
    m_vertices[2] = { Vector3(-5.0f ,5.0f ,-5.0f),Vector4(1,1,1,1), Vector2(0.0f, 1.0f) };	//����
    m_vertices[3] = { Vector3( 5.0f ,5.0f ,-5.0f),Vector4(1,1,1,1), Vector2(1.0f, 1.0f) };	    //�E��


    DirectX::CreateWICTextureFromFile(
        m_deviceResources->GetD3DDevice(),		// �f�o�C�X�R���e�L�X�g
        L"Resources/Textures/Daylight_Box.png",	// �摜�t�@�C���̃p�X
        nullptr,								// �����I�ȃe�N�X�`��
        m_texture.ReleaseAndGetAddressOf()		// �V�F�[�_���\�[�X�r���[(�\���p)
    );

    
}

void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    const auto& gp = m_inputManager->GetGamePadTracker();

    m_player->Update();

    if (gp->a == gp->PRESSED)
    {
        m_enemyNum++;
        if (m_enemyNum >= m_enemy.size())
        {
            m_enemyNum = 0;
        }
        m_player->SetTarget(m_enemy.at(m_enemyNum));
    }

}

void PlayScene::Render()
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();


    m_player->Render();

    for (auto& enemy: m_enemy)
    {
        enemy->Render();
    }


    // �v���~�e�B�u�o�b�`�ŕ`�悷��
    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
    m_primitiveBatch->End();

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