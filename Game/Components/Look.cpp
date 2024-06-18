#include "pch.h"
#include "Look.h"

Look::Look()
{
}

Look::~Look()
{
}

void Look::Initialize()
{
}

void Look::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    UNREFERENCED_PARAMETER(elapsedTime);

    // �v���C���[�̌����Ă���������v�Z
    Vector3 dot = GetOwner()->GetPosition() - m_targetPosition;
    float radianY = atan2f(dot.x, dot.z);

    // �v���C���[�̌����Ă��������ݒ�
    Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(0, radianY, 0));
    GetOwner()->SetQuaternion(quaternion);

    // �^�[�Q�b�g�̍��W���X�V
    m_targetPosition = Vector3::Lerp(m_targetPosition, m_enemy->GetPosition(), 0.1f);
}

void Look::Render()
{

}

void Look::Finalize()
{

}

void Look::SetTarget(GameObject* player, GameObject* enemy)
{
    m_player = player;
    m_enemy  = enemy;
}