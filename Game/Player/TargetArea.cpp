#include "pch.h"
#include "TargetArea.h"
#include "Game/Player/Player.h"

TargetArea::TargetArea()
{
    using namespace DirectX;

    m_graphics = Graphics::GetInstance();

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    // 画像をロードする
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(
            device,
            L"Resources/Textures/3.png",
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );

    SetScale(5.f);
    m_rotate = 0;
    m_range = 200.f;
    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();
    // SetScale(GetScale() - 0.01f);

    m_aspect = float(rect.right) / 1280;

    m_range = m_range * m_aspect;
}

TargetArea::~TargetArea()
{


}

void TargetArea::Initialize()
{

}

bool TargetArea::Update(Player* player , GameObject* target)
{
    using namespace DirectX::SimpleMath;

    UNREFERENCED_PARAMETER(player);

    Matrix view = m_graphics->GetViewMatrix();
    Matrix proj = m_graphics->GetProjectionMatrix();

    m_scale = lerp(m_scale, SCALE, 0.05f);

    // ビューポート行列（スクリーン行列）の作成
    float w;
    float h;

    //m_graphics->GetScreenSize(w, h);
    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();
    // SetScale(GetScale() - 0.01f);

    w = rect.right / 2.f;
    h = rect.bottom / 2.f;


    Matrix viewport = {
        float(w) , 0 , 0 , 0 ,
        0 ,float(-h) , 0 , 0 ,
        0 , 0 , 1 , 0 ,
        float(w) , float(h) , 0 , 1
    };

    Vector3 screenPos, tmp = target->GetPosition();
    // ビュー変換とプロジェクション変換
    tmp = Vector3::Transform(tmp, view);
    tmp = Vector3::Transform(tmp, proj);
    // zで割って-1~1の範囲に収める
    tmp.x /= tmp.z;
    tmp.y /= tmp.z;
    tmp.z /= tmp.z;
    // スクリーン変換
    screenPos = Vector3::Transform(tmp, viewport);

    float x = w - screenPos.x;
    float y = h - screenPos.y;

    // ターゲット範囲にいるか
    if ((x * x) + (y * y) <= m_range * m_range)
    {
        //player->SetTarget(target);
        return true;
    }

    return false;


}

void TargetArea::Render(bool inArea)
{
    using namespace DirectX::SimpleMath;


    RECT windowsize = m_graphics->GetDeviceResources()->GetOutputSize();
     
    float w = windowsize.right;
    float h = windowsize.bottom;

    w /= 2;
    h /= 2;

    float n = float(windowsize.right) / float(1200);

    RECT rect = { 0,0,int(m_textureSize.x) ,int(m_textureSize.y) };

    // 画面の中心に円を表示
    m_spriteBatch->Begin();
    if (inArea)
    {
        m_rotate = lerp(m_rotate, 0, 0.1f);
        m_spriteBatch->Draw(m_texture.Get(), Vector2(float(w), float(h)), &rect,
            DirectX::Colors::Red,
            DirectX::XMConvertToRadians(m_rotate),
            Vector2(m_textureSize.x / 2, m_textureSize.y / 2),
            Vector2(m_scale * n, m_scale * n)
        );
    }
    else
    {
        m_rotate++;
        m_spriteBatch->Draw(m_texture.Get(), Vector2(float(w), float(h)), &rect,
            DirectX::Colors::CadetBlue,
            DirectX::XMConvertToRadians(m_rotate),
            Vector2(m_textureSize.x / 2, m_textureSize.y / 2),
            Vector2(m_scale * n, m_scale * n)
        );
    }

    m_spriteBatch->End();
}

void TargetArea::Finalize()
{

}
