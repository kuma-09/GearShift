#include "pch.h"
#include "Player.h"
#include "Game/Enemy/Enemy.h"
#include "Particle.h"
#include "Framework/Microsoft/DebugDraw.h"
#include "cmath"

void Player::Initialize()
{
	using namespace DirectX::SimpleMath;
	using namespace DirectX;

	m_graphics = Graphics::GetInstance();
	m_deviceResources = m_graphics->GetDeviceResources();
	m_inputManager = InputManager::GetInstance();

    m_resources = Resources::GetInstance();

	m_position = Vector3(5, 0, 5);
    m_quaternion = Quaternion::Identity;


	m_boundingBox = std::make_unique<BoundingBox>();
	m_boundingBox->Extents = Vector3(0.5f, 0.5f, 0.5f);

    m_particle = new Particle();
    m_particle->Initialize();

    m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(m_deviceResources->GetD3DDeviceContext());
}

void Player::Update(float elapseTime)
{
    using namespace DirectX::SimpleMath;

    const auto& kb = m_inputManager->GetKeyboardState();
    const auto& gp = m_inputManager->GetGamePadTracker();


    Vector3 dot = m_position - m_targetEnemy->GetPosition();
    float radian = atan2f(dot.x, dot.z);


    m_quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(0, radian, 0));

    const auto& gpstate = m_inputManager->GetGamePadState();

    

    Vector3 velocity = Vector3::Zero;

    

    if (gpstate.thumbSticks.leftY != 0)
    {
        velocity += SPEED_FB * Matrix::CreateFromQuaternion(m_quaternion).Forward() * elapseTime * gpstate.thumbSticks.leftY;
    }
    if (gpstate.thumbSticks.leftX != 0)
    {
        velocity += SPEED_RL * Matrix::CreateFromQuaternion(m_quaternion).Right() * elapseTime * gpstate.thumbSticks.leftX;
    }

    if (kb.Right)
    {
        velocity += SPEED_RL * dot.Length() * Matrix::CreateFromQuaternion(m_quaternion).Right();

    }

    velocity.Normalize();
    m_position += velocity / 5;

    m_particle->Update(elapseTime);

    dot = m_position - m_targetEnemy->GetPosition();
    radian = atan2f(dot.x, dot.z);

    // 回転行列を生成する
    m_quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(0, radian, 0));


    // ビュー行列を作成
    Vector3 eye = m_position + 15 * -Matrix::CreateFromQuaternion(m_quaternion).Forward() + 5 * Matrix::CreateFromQuaternion(m_quaternion).Up();
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

    Matrix world = Matrix::Identity;

    Vector3 dot = m_position - m_targetEnemy->GetPosition();
    float radian = atan2f(dot.x, dot.z);

    Quaternion q = Quaternion::CreateFromYawPitchRoll(Vector3(0, radian, 0));
    world = Matrix::CreateFromQuaternion(q);
    world *= Matrix::CreateTranslation(m_position);


    m_particle->Render(world);

    m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());




    Vector3 tmp(world._41, world._42, world._43);
    m_boundingBox->Center = tmp;

    m_resources->GetModel()->Draw(context, *m_graphics->GetCommonStates(), world, m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    m_graphics->DrawPrimitiveEnd();

    m_graphics->DrawPrimitivePositionColorBegin(view, projection);
    DX::Draw(m_graphics->GetPrimitiveBatchPositionColor(), *m_boundingBox.get());
    m_graphics->DrawPrimitivePositionColorEnd();



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
    m_quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(0, radian, 0));


    // ビュー行列を作成
    Vector3 eye = m_position + 15 * -Matrix::CreateFromQuaternion(m_quaternion).Forward() + 5 * Matrix::CreateFromQuaternion(m_quaternion).Up();
    Vector3 target = m_targetEnemy->GetPosition();
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);
}