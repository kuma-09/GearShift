#include "pch.h"
#include "Player.h"
#include "Game/Enemy/Enemy.h"
#include "cmath"

void Player::Initialize()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	m_deviceResources = m_graphics->GetDeviceResources();
	m_inputManager = InputManager::GetInstance();

    m_resources = Resources::GetInstance();


	m_angle = 0;
	m_position = Vector3(5, 0, 5);

    m_center = Vector3(-3, 0, 20);

	m_boundingBox = std::make_unique<BoundingBox>();
	m_boundingBox->Extents = Vector3(0.5f, 0.5f, 0.5f);
}

void Player::Update()
{
    using namespace DirectX::SimpleMath;

    const auto& gp = m_inputManager->GetGamePadTracker();

    Vector3 dot = m_position - m_targetEnemy->GetPosition();
    float radian = atan2f(dot.x, dot.z);

    // 回転行列を生成する
    Matrix rotation = Matrix::CreateRotationY(radian);
    Vector3 velocity = Vector3::Zero;





    if (gp->dpadUp)
    {
        velocity += SPEED_FB * rotation.Forward();


    }
    if (gp->dpadDown)
    {
        velocity += SPEED_FB * -rotation.Forward();


    }
    if (gp->dpadRight)
    {
        velocity += SPEED_RL * dot.Length() * rotation.Right();

        //m_angle -= 0.05f;
    }
    if (gp->dpadLeft)
    {
        velocity += SPEED_RL * dot.Length() * -rotation.Right();

        //m_angle += 0.05f;
    }
    m_position += velocity;


    dot = m_position - m_targetEnemy->GetPosition();
    radian = atan2f(dot.x, dot.z);

    // 回転行列を生成する
    rotation = Matrix::CreateRotationY(radian);

    // ビュー行列を作成
    Vector3 eye = m_position + 10 * -rotation.Forward() + 5 * rotation.Up();
    Vector3 target = m_targetEnemy->GetPosition();
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);

}

void Player::Render()
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();


    m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    Matrix world = Matrix::Identity;

    Vector3 dot = m_position - m_targetEnemy->GetPosition();
    float radian = atan2f(dot.x, dot.z);

    world = Matrix::CreateRotationY(radian);
    world *= Matrix::CreateTranslation(m_position);


    Vector3 tmp(world._41, 0, world._43);

    m_boundingBox->Center = tmp;

    m_resources->GetModel()->Draw(context, *m_graphics->GetCommonStates(), world, m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    m_graphics->DrawPrimitiveEnd();
}

void Player::Finalize()
{
    delete m_graphics;
    m_graphics = nullptr;
    delete m_deviceResources;
    m_deviceResources = nullptr;
    delete m_inputManager;
    m_inputManager = nullptr;
}

void Player::SetTarget(Enemy* enemy)
{
    using namespace DirectX::SimpleMath;

    m_targetEnemy = enemy;

    Vector3 dot = m_position - m_targetEnemy->GetPosition();
    float radian = atan2f(dot.x, dot.z);

    // 回転行列を生成する
    Matrix rotation = Matrix::CreateRotationY(radian);

    // ビュー行列を作成
    Vector3 eye = m_position + 10 * -rotation.Forward() + 5 * rotation.Up();
    Vector3 target = m_targetEnemy->GetPosition();
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);
}