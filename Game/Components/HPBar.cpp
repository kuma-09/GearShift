#include "pch.h"
#include "HPBar.h"
#include "HP.h"
#include "Game/GameObject.h"


HPBar::HPBar()
    :m_vertex{}
{
    using namespace DirectX;

    m_graphics = Graphics::GetInstance();
    auto deviceResources = m_graphics->GetDeviceResources();
    m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(deviceResources->GetD3DDeviceContext());
    m_basicEffect = std::make_unique<BasicEffect>(deviceResources->GetD3DDevice());
    m_texture = Resources::GetInstance()->GetGreenTexture();
}

HPBar::~HPBar()
{

}

void HPBar::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;
    m_maxHp = GetOwner()->GetComponent<HP>()->GetHP();

    // ��
    m_vertex[0] = VertexPositionColorTexture(Vector3(0.0f, -0.03f, 0), Vector4(1, 1, 1, 1), Vector2(0, 0));
    m_vertex[1] = VertexPositionColorTexture(Vector3(0.0f, -0.03f, 0), Vector4(1, 1, 1, 1), Vector2(1, 0));
    m_vertex[2] = VertexPositionColorTexture(Vector3(0.0f, 0.03f, 0), Vector4(1, 1, 1, 1), Vector2(0, 1));
    m_vertex[3] = VertexPositionColorTexture(Vector3(0.0f, 0.03f, 0), Vector4(1, 1, 1, 1), Vector2(1, 1));

    // ��
    m_backVertex[0] = VertexPositionColorTexture(Vector3(0.0f, -0.05f, 0), Vector4(1, 1, 1, 1), Vector2(0, 0));
    m_backVertex[1] = VertexPositionColorTexture(Vector3(0.0f, -0.05f, 0), Vector4(1, 1, 1, 1), Vector2(1, 0));
    m_backVertex[2] = VertexPositionColorTexture(Vector3(0.0f, 0.05f, 0), Vector4(1, 1, 1, 1), Vector2(0, 1));
    m_backVertex[3] = VertexPositionColorTexture(Vector3(0.0f, 0.05f, 0), Vector4(1, 1, 1, 1), Vector2(1, 1));
}

void HPBar::Update(float elapsedTime)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    UNREFERENCED_PARAMETER(elapsedTime);

}

void HPBar::Render(DirectX::SimpleMath::Vector2 position)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto state = m_graphics->GetCommonStates();

    context->OMSetBlendState(state->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(state->DepthDefault(), 0);
    context->RSSetState(state->CullNone());

    float barSize = 0;

    if (GetOwner()->GetComponent<HP>()->GetHP())
    {
        barSize = (float)GetOwner()->GetComponent<HP>()->GetHP() / (float)m_maxHp;
        barSize *= 0.14f * 2;
        barSize = std::max(barSize, 0.0f);
    }


    // ��
    m_vertex[0].position = Vector3(-0.14f         , -0.03f, 0);
    m_vertex[1].position = Vector3(barSize - 0.14f, -0.03f, 0);
    m_vertex[2].position = Vector3(-0.14f         ,  0.03f, 0);
    m_vertex[3].position = Vector3(barSize - 0.14f,  0.03f, 0);

    // ��
    m_backVertex[0].position = Vector3(-0.15f, -0.05f, 0);
    m_backVertex[1].position = Vector3( 0.15f, -0.05f, 0);
    m_backVertex[2].position = Vector3(-0.15f,  0.05f, 0);
    m_backVertex[3].position = Vector3( 0.15f,  0.05f, 0);

    // �r���[�s���ݒ肷��
    m_basicEffect->SetView(Matrix::Identity);
    // �v���W�F�N�V�����s���ݒ肷��
    m_basicEffect->SetProjection(Matrix::Identity);
    // ���[���h�s���ݒ肷��
    m_basicEffect->SetWorld(Matrix::CreateTranslation({position.x,position.y, 0}));

    // ���_�J���[��L���ɂ���
    m_basicEffect->SetVertexColorEnabled(true);
    // �e�N�X�`����L���ɂ���
    m_basicEffect->SetTextureEnabled(true);

    m_basicEffect->SetColorAndAlpha({ 1,1,1,0 });

    // ���̓��C�A�E�g��ݒ肷��
    m_basicEffect->Apply(context);
    // ���̓��C�A�E�g��ݒ肷��
    context->IASetInputLayout(m_graphics->GetInputLayout());
    // �v���~�e�B�u�o�b�`���J�n����
    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_backVertex[0], m_backVertex[1], m_backVertex[3], m_backVertex[2]);
    m_primitiveBatch->End();

    // ���_�J���[��ύX
    m_basicEffect->SetColorAndAlpha({ 1,1,1,1 });
    // �e�N�X�`����L���ɂ���
    m_basicEffect->SetTexture(m_texture);
    // ���̓��C�A�E�g��ݒ肷��
    m_basicEffect->Apply(context);

    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
    m_primitiveBatch->End();
}

void HPBar::Render(DirectX::SimpleMath::Vector3 position)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto state   = m_graphics->GetCommonStates();

    auto view = m_graphics->GetViewMatrix();
    auto proj = m_graphics->GetProjectionMatrix();

    context->OMSetBlendState(state->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(state->DepthDefault(), 0);
    context->RSSetState(state->CullNone());

    float barSize = 0;

    if (GetOwner()->GetComponent<HP>()->GetHP())
    {
        barSize = (float)GetOwner()->GetComponent<HP>()->GetHP() / (float)m_maxHp;
        barSize *= 4;
    }

    // ��
    m_vertex[0].position = Vector3(-2, 0, 0);
    m_vertex[1].position = Vector3(barSize - 2, 0, 0);
    m_vertex[2].position = Vector3(-2, 0.5f, 0);
    m_vertex[3].position = Vector3(barSize - 2, 0.5f, 0);

    // ��
    m_backVertex[0].position = Vector3(-2.1f, -0.1f, 0);
    m_backVertex[1].position = Vector3(2.1f, -0.1f, 0);
    m_backVertex[2].position = Vector3(-2.1f, 0.6f, 0);
    m_backVertex[3].position = Vector3(2.1f, 0.6f, 0);

    Matrix billboard = view.Invert();
    billboard._41 = 0;
    billboard._42 = 0;
    billboard._43 = 0;

    billboard *= Matrix::CreateTranslation(position + Vector3(0,2,0));

    // �r���[�s���ݒ肷��
    m_basicEffect->SetView(view);
    // �v���W�F�N�V�����s���ݒ肷��
    m_basicEffect->SetProjection(proj);
    // ���[���h�s���ݒ肷��
    m_basicEffect->SetWorld(billboard);

    // ���_�J���[��L���ɂ���
    m_basicEffect->SetVertexColorEnabled(true);
    // �e�N�X�`����L���ɂ���
    m_basicEffect->SetTextureEnabled(true);

    m_basicEffect->SetColorAndAlpha({1,1,1,0});

    // ���̓��C�A�E�g��ݒ肷��
    m_basicEffect->Apply(context);
    // ���̓��C�A�E�g��ݒ肷��
    context->IASetInputLayout(m_graphics->GetInputLayout());
    // �v���~�e�B�u�o�b�`���J�n����
    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_backVertex[0], m_backVertex[1], m_backVertex[3], m_backVertex[2]);
    m_primitiveBatch->End();

    // ���_�J���[��ύX
    m_basicEffect->SetColorAndAlpha({ 1,1,1,1 });
    // �e�N�X�`����L���ɂ���
    m_basicEffect->SetTexture(m_texture);
    // ���̓��C�A�E�g��ݒ肷��
    m_basicEffect->Apply(context);

    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
    m_primitiveBatch->End();

}

void HPBar::Finalize()
{

}