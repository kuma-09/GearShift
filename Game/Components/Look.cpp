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

    if (m_to)
    {
        // プレイヤーの向いている方向を計算
        Vector3 direction = GetOwner()->GetPosition() - m_targetPosition;
        float radianY = atan2f(direction.x, direction.z);
        float radianX = -atan2f(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z));

        // プレイヤーの向いている方向を設定
        Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(Vector3(radianX, radianY, 0));
        GetOwner()->SetQuaternion(Quaternion::Lerp(GetOwner()->GetQuaternion(), quaternion,0.1f));

        // ターゲットの座標を更新
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