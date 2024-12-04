#include "pch.h"
#include "Look.h"

Look::Look()
{
    m_h = false;
    m_v = false;
}

Look::~Look()
{
}

void Look::Initialize(bool h, bool v)
{
    m_h = h;
    m_v = v;
}

void Look::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;


    if (m_to)
    {
        // �v���C���[�̌����Ă���������v�Z
        Vector3 direction = GetOwner()->GetPosition() - m_targetPosition;
        float radianY = atan2f(direction.x, direction.z);
        float radianX = -atan2f(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z));

        if (m_h) radianY = 0;
        if (m_v) radianX = 0;

        // �v���C���[�̌����Ă��������ݒ�
        Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Vector3( radianX, radianY, 0));
        GetOwner()->SetQuaternion(Quaternion::Lerp(GetOwner()->GetQuaternion(), quaternion,elapsedTime * 10));

        // �^�[�Q�b�g�̍��W���X�V
        m_targetPosition = Vector3::Lerp(m_targetPosition, m_to->GetPosition(), 0.1f);
    }
}

void Look::Render()
{

}

void Look::Finalize()
{

}

void Look::SetTarget(GameObject* from, GameObject* to)
{
    m_from = from;
    m_to  = to;
}