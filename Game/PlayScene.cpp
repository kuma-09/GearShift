#include "pch.h"
#include "PlayScene.h"
#include "cmath"

void PlayScene::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    m_model = std::make_unique<DirectX::Model>();
    m_model = DirectX::Model::CreateFromCMO(m_graphics->GetDeviceResources()->GetD3DDevice(), L"Resources/Models/dice.cmo", *m_graphics->GetFX());

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

    m_angle = 0;
    m_position = Vector3(0,0,5);
    m_center = Vector3(-3,0,20);

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

    Vector3 dot = m_position - m_center;
    float radian = atan2f(dot.x, dot.z);

    // ��]�s��𐶐�����
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

        m_angle -= 0.05f;
    }
    if (gp->dpadLeft)
    {
        velocity += SPEED_RL * dot.Length() * -rotation.Right();
        
        m_angle += 0.05f;
    }
    m_position += velocity;

    
    dot = m_position - m_center;
    radian = atan2f(dot.x, dot.z);

    // ��]�s��𐶐�����
    rotation = Matrix::CreateRotationY(radian);

    // �r���[�s����쐬
    Vector3 eye = m_position + 10 * -rotation.Forward() + 5 * rotation.Up();
    Vector3 target = m_center;
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);
}

void PlayScene::Render()
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();


    m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    Matrix world = Matrix::Identity;
    
    Vector3 dot = m_position - m_center;
    float radian = atan2f(dot.x, dot.z);
    
    world = Matrix::CreateRotationY(radian);
    world *= Matrix::CreateTranslation(m_position);

    // �G�t�F�N�g��ύX���遨���W�n��ݒ肷��
    m_basicEffect->SetWorld(world);				// ���[���h�s��
    m_basicEffect->SetView(view);				// �r���[�s��
    m_basicEffect->SetProjection(projection);	// �ˉe�s��
    m_basicEffect->SetTexture(m_texture.Get());	// �e�N�X�`��
    m_basicEffect->Apply(context);				// �x�[�V�b�N�G�t�F�N�g���X�V����


    m_model->Draw(context, *m_graphics->GetCommonStates(), world, m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    m_model->Draw(context, *m_graphics->GetCommonStates(), Matrix::CreateTranslation(m_center), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    m_model->Draw(context, *m_graphics->GetCommonStates(), Matrix::CreateTranslation(m_center + Vector3(0,0,5)), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());


    m_graphics->DrawPrimitiveEnd();



    // �v���~�e�B�u�o�b�`�ŕ`�悷��
    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
    m_primitiveBatch->End();

}

void PlayScene::Finalize()
{

}