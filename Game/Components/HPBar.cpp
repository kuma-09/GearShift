#include "pch.h"
#include "HPBar.h"
#include "HP.h"
#include "Game/GameObject.h"
#include "Framework/Easing.h"


HPBar::HPBar()
    :m_vertex{}
{
    using namespace DirectX;

    m_graphics = Graphics::GetInstance();
    auto deviceResources = m_graphics->GetDeviceResources();
    m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(deviceResources->GetD3DDeviceContext());
    m_basicEffect = std::make_unique<BasicEffect>(deviceResources->GetD3DDevice());
    m_texture = Resources::GetInstance()->GetWhiteTexture();
}

HPBar::~HPBar()
{

}

void HPBar::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;
    m_maxHp = GetOwner()->GetComponent<HP>()->GetHP();
    m_redHp = (float)GetOwner()->GetComponent<HP>()->GetHP() / (float)m_maxHp * 4;
    m_nowTime = 0;

    // 緑
    m_vertex[0] = VertexPositionColorTexture(Vector3(0.0f, -0.03f, 0), Vector4(1, 1, 1, 1), Vector2(0, 0));
    m_vertex[1] = VertexPositionColorTexture(Vector3(0.0f, -0.03f, 0), Vector4(1, 1, 1, 1), Vector2(1, 0));
    m_vertex[2] = VertexPositionColorTexture(Vector3(0.0f, 0.03f, 0), Vector4(1, 1, 1, 1), Vector2(0, 1));
    m_vertex[3] = VertexPositionColorTexture(Vector3(0.0f, 0.03f, 0), Vector4(1, 1, 1, 1), Vector2(1, 1));

    // 赤
    m_redVertex[0] = VertexPositionColorTexture(Vector3(0.0f, -0.03f, 0), Vector4(1, 1, 1, 1), Vector2(0, 0));
    m_redVertex[1] = VertexPositionColorTexture(Vector3(0.0f, -0.03f, 0), Vector4(1, 1, 1, 1), Vector2(1, 0));
    m_redVertex[2] = VertexPositionColorTexture(Vector3(0.0f, 0.03f, 0), Vector4(1, 1, 1, 1), Vector2(0, 1));
    m_redVertex[3] = VertexPositionColorTexture(Vector3(0.0f, 0.03f, 0), Vector4(1, 1, 1, 1), Vector2(1, 1));

    // 黒
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
    float barSize = 0;

    if (GetOwner()->GetComponent<HP>()->GetHP())
    {
        barSize = (float)GetOwner()->GetComponent<HP>()->GetHP() / (float)m_maxHp;
        barSize *= 4;
    }

    m_greenHp = barSize;
    if (m_greenHp <= m_redHp)
    {
        m_redHp -= elapsedTime;
    }
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

    float BarSize2D = 0.14f * 2 / 4;

    float GreenHPBar = std::max(m_greenHp * BarSize2D,0.0f);
    float RedHPBar   = std::max(m_redHp * BarSize2D, 0.0f);


    // 緑
    m_vertex[0].position = Vector3(-0.14f         , -0.03f, 0);
    m_vertex[1].position = Vector3(GreenHPBar - 0.14f, -0.03f, 0);
    m_vertex[2].position = Vector3(-0.14f         ,  0.03f, 0);
    m_vertex[3].position = Vector3(GreenHPBar - 0.14f,  0.03f, 0);

    // 赤
    m_redVertex[0].position = Vector3(-0.14f, -0.03f, 0);
    m_redVertex[1].position = Vector3(RedHPBar - 0.14f, -0.03f, 0);
    m_redVertex[2].position = Vector3(-0.14f, 0.03f, 0);
    m_redVertex[3].position = Vector3(RedHPBar - 0.14f, 0.03f, 0);

    // 黒
    m_backVertex[0].position = Vector3(-0.15f, -0.05f, 0);
    m_backVertex[1].position = Vector3( 0.15f, -0.05f, 0);
    m_backVertex[2].position = Vector3(-0.15f,  0.05f, 0);
    m_backVertex[3].position = Vector3( 0.15f,  0.05f, 0);

    // ビュー行列を設定する
    m_basicEffect->SetView(Matrix::Identity);
    // プロジェクション行列を設定する
    m_basicEffect->SetProjection(Matrix::Identity);
    // ワールド行列を設定する
    m_basicEffect->SetWorld(Matrix::CreateTranslation({position.x,position.y, 0}));

    // 頂点カラーを有効にする
    m_basicEffect->SetVertexColorEnabled(true);
    // テクスチャを有効にする
    m_basicEffect->SetTextureEnabled(true);

    m_basicEffect->SetColorAndAlpha({ 1,1,1,0 });

    // 入力レイアウトを設定する
    m_basicEffect->Apply(context);
    // 入力レイアウトを設定する
    context->IASetInputLayout(m_graphics->GetInputLayout());
    // プリミティブバッチを開始する
    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_backVertex[0], m_backVertex[1], m_backVertex[3], m_backVertex[2]);
    m_primitiveBatch->End();

    // 頂点カラーを変更
    m_basicEffect->SetColorAndAlpha({ 1,0,0,1 });
    // テクスチャを有効にする
    m_basicEffect->SetTexture(m_texture);
    // 入力レイアウトを設定する
    m_basicEffect->Apply(context);

    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_redVertex[0], m_redVertex[1], m_redVertex[3], m_redVertex[2]);
    m_primitiveBatch->End();

    // 頂点カラーを変更
    m_basicEffect->SetColorAndAlpha({ 0,1,0,1 });
    // テクスチャを有効にする
    m_basicEffect->SetTexture(m_texture);
    // 入力レイアウトを設定する
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


    // 緑
    m_vertex[0].position = Vector3(-2, 0, 0);
    m_vertex[1].position = Vector3(m_greenHp - 2, 0, 0);
    m_vertex[2].position = Vector3(-2, 0.5f, 0);
    m_vertex[3].position = Vector3(m_greenHp - 2, 0.5f, 0);

    // 赤
    m_redVertex[0].position = Vector3(-2, 0, 0);
    m_redVertex[1].position = Vector3(m_redHp - 2, 0, 0);
    m_redVertex[2].position = Vector3(-2, 0.5f, 0);
    m_redVertex[3].position = Vector3(m_redHp - 2, 0.5f, 0);


    // 黒（外枠
    m_backVertex[0].position = Vector3(-2.1f, -0.1f, 0);
    m_backVertex[1].position = Vector3(2.1f, -0.1f, 0);
    m_backVertex[2].position = Vector3(-2.1f, 0.6f, 0);
    m_backVertex[3].position = Vector3(2.1f, 0.6f, 0);

    Matrix billboard = view.Invert();
    billboard._41 = 0;
    billboard._42 = 0;
    billboard._43 = 0;

    billboard *= Matrix::CreateTranslation(position + Vector3(0,2,0));

    // ビュー行列を設定する
    m_basicEffect->SetView(view);
    // プロジェクション行列を設定する
    m_basicEffect->SetProjection(proj);
    // ワールド行列を設定する
    m_basicEffect->SetWorld(billboard);

    // 頂点カラーを有効にする
    m_basicEffect->SetVertexColorEnabled(true);
    // テクスチャを有効にする
    m_basicEffect->SetTextureEnabled(true);

    m_basicEffect->SetColorAndAlpha({1,1,1,0});

    // 入力レイアウトを設定する
    m_basicEffect->Apply(context);
    // 入力レイアウトを設定する
    context->IASetInputLayout(m_graphics->GetInputLayout());
    // プリミティブバッチを開始する
    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_backVertex[0], m_backVertex[1], m_backVertex[3], m_backVertex[2]);
    m_primitiveBatch->End();

    // 頂点カラーを変更
    m_basicEffect->SetColorAndAlpha({ 1,0,0,1 });
    // テクスチャを有効にする
    m_basicEffect->SetTexture(m_texture);
    // 入力レイアウトを設定する
    m_basicEffect->Apply(context);

    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_redVertex[0], m_redVertex[1], m_redVertex[3], m_redVertex[2]);
    m_primitiveBatch->End();

    // 頂点カラーを変更
    m_basicEffect->SetColorAndAlpha({ 0,1,0,1 });
    // テクスチャを有効にする
    m_basicEffect->SetTexture(m_texture);
    // 入力レイアウトを設定する
    m_basicEffect->Apply(context);

    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_vertex[0], m_vertex[1], m_vertex[3], m_vertex[2]);
    m_primitiveBatch->End();

}
