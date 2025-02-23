#include "pch.h"
#include "TargetArea.h"
#include "Game/Components/HPBar.h"

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
            L"Resources/Textures/Reticle.png",
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );

    SetScale(20.f);
    m_rotate = 0;
    m_range = 200.f;
    RECT rect = m_graphics->GetDeviceResources()->GetOutputSize();

    m_aspect = float(rect.right) / 1280;

    m_range = m_range * m_aspect;
}

TargetArea::~TargetArea()
{


}

void TargetArea::Initialize()
{
    m_target = nullptr;
}

void TargetArea::ClearTarget()
{
    m_target = nullptr;
    m_tmpInRange = 0;
}

bool TargetArea::Update(GameObject* player, GameObject* target,GameObject* camera)
{
    using namespace DirectX::SimpleMath;




    SetScale(lerp(GetScale(), SCALE, 0.05f));

    Vector2 screenPos = GetScreenPosition(target);
    float x = screenPos.x;
    float y = screenPos.y;

    Vector3 pPos = Vector3::Transform(Vector3::Forward, camera->GetQuaternion());
    Vector3 tPos = player->GetPosition() - target->GetPosition();

    // �v���C���[�̑O�㔻��
    float dot = pPos.Dot(tPos);

    float inRange = ((x * x) + (y * y)) - (m_range * m_range);

    // �^�[�Q�b�g�͈͂ɂ��邩
    if (inRange <= 0 && dot < 0.5f)
    {
        // �ł����S�ɋ߂�������X�V
        if (inRange < m_tmpInRange)
        {
            m_tmpInRange = inRange;
            m_target = target;
        }
        return true;
    }

    return false;

}

void TargetArea::Render(GameObject* target)
{
    _Unreferenced_parameter_(target);
    using namespace DirectX::SimpleMath;


    RECT windowsize = m_graphics->GetDeviceResources()->GetOutputSize();
     
    float w = float(windowsize.right);
    float h = float(windowsize.bottom);

    w /= 2;
    h /= 2;

    float n = float(windowsize.right) / float(1280);

    RECT rect = { 0,0,int(m_textureSize.x) ,int(m_textureSize.y) };



    // ��ʂ̒��S�ɉ~��\��
    m_spriteBatch->Begin();
    if (m_target)
    {
        Vector2 screenPos = GetScreenPosition(m_target);
        m_rotate = lerp(m_rotate, 0, 0.1f);
        m_spriteBatch->Draw(m_texture.Get(), Vector2(float(w), float(h)), &rect,
            DirectX::Colors::Red,
            DirectX::XMConvertToRadians(m_rotate),
            Vector2(m_textureSize.x / 2, m_textureSize.y / 2),
            Vector2(m_scale * n, m_scale * n)
        );

        m_spriteBatch->Draw(m_texture.Get(), Vector2(float(w), float(h)) - screenPos, &rect, DirectX::Colors::CadetBlue,
            DirectX::XMConvertToRadians(m_rotate),
            Vector2(m_textureSize.x / 2, m_textureSize.y / 2),
            Vector2(GetScale() * 0.2f * n, GetScale() * 0.2f * n)
        );

        m_target->GetComponent<HPBar>()->Render(m_target->GetPosition());

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

DirectX::SimpleMath::Vector2 TargetArea::GetScreenPosition(GameObject* target)
{
    using namespace DirectX::SimpleMath;

    Matrix view = m_graphics->GetViewMatrix();
    Matrix proj = m_graphics->GetProjectionMatrix();

    // �r���[�|�[�g�s��i�X�N���[���s��j�̍쐬
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

    return { x,y };
}
