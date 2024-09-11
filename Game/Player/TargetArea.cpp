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

    // �摜�����[�h����
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(
            device,
            L"Resources/Textures/Circle.png",
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );

}

TargetArea::~TargetArea()
{


}

void TargetArea::Initialize()
{
    // �^�[�Q�b�g�����b�N�ł���͈�
    m_range = 100;
}

bool TargetArea::Update(Player* player , GameObject* target)
{
    using namespace DirectX::SimpleMath;

    Matrix view = m_graphics->GetViewMatrix();
    Matrix proj = m_graphics->GetProjectionMatrix();

    // �r���[�|�[�g�s��i�X�N���[���s��j�̍쐬
    int w;
    int h;

    m_graphics->GetScreenSize(w, h);

    w /= 2;
    h /= 2;

    Matrix viewport = {
        float(w) , 0 , 0 , 0 ,
        0 ,float(-h) , 0 , 0 ,
        0 , 0 , 1 , 0 ,
        float(w) , float(h) , 0 , 1
    };

    Vector3 screenPos, tmp = target->GetPosition();
    // �r���[�ϊ��ƃv���W�F�N�V�����ϊ�
    tmp = Vector3::Transform(tmp, view);
    tmp = Vector3::Transform(tmp, proj);
    // z�Ŋ�����-1~1�͈̔͂Ɏ��߂�
    tmp.x /= tmp.z;
    tmp.y /= tmp.z;
    tmp.z /= tmp.z;
    // �X�N���[���ϊ�
    screenPos = Vector3::Transform(tmp, viewport);

    float x = w - screenPos.x;
    float y = h - screenPos.y;

    // �^�[�Q�b�g�͈͂ɂ��邩
    if ((x * x) + (y * y) <= m_range * m_range)
    {
        // �͈͂ɂ����G���^�[�Q�b�g�Ƃ��ēK�p
        player->SetTarget(target);
        return true;
    }

    return false;

}

void TargetArea::Render()
{
    using namespace DirectX::SimpleMath;

    int w;
    int h;

    m_graphics->GetScreenSize(w, h);

    w /= 2;
    h /= 2;

    // ��ʂ̒��S�ɉ~��\��
    m_spriteBatch->Begin();
    m_spriteBatch->Draw(m_texture.Get(),Vector2(w,h) - Vector2(250,250));
    m_spriteBatch->End();
}

void TargetArea::Finalize()
{

}
