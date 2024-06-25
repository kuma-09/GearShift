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
    Vector3 direction = GetOwner()->GetPosition() - m_targetPosition;
    //Vector3 direction = m_targetPosition - GetOwner()->GetPosition();
    float radianY = atan2f(direction.x, direction.z);
    float radianX = -atan2f(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z));

    // �v���C���[�̌����Ă��������ݒ�
    Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(radianX, radianY, 0));
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