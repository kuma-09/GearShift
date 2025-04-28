#include "pch.h"
#include "Collider.h"
#include "Game/Scene.h"
#include "Game/GameObject.h"
#include "Framework/Microsoft/DebugDraw.h"
#include "Game/PlayScene.h"
#include "Game/Manager/CollisionManager.h"

#include "Physics.h"

// �R���X�g���N�^
Collider::Collider()
{
	m_graphics = Graphics::GetInstance();
	m_boudingBox = std::make_unique<DirectX::BoundingBox>();
	m_boudingBox->Center = DirectX::SimpleMath::Vector3::Zero;
	m_boudingBox->Extents = DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f);
    m_initalePosition = DirectX::SimpleMath::Vector3::Zero;
    CollisionManager::Add(this);
    SetActive(false);
}

// �f�X�g���N�^
Collider::~Collider()
{
    CollisionManager::Remove(this);
}

/// <summary>
/// ����������
/// </summary>
/// <param name="id">���ʗp�^�O</param>
/// <param name="colliderType">�����蔻��̎��</param>
/// <param name="size">�傫��</param>
/// <param name="initialPos">�����ʒu</param>
void Collider::Initialize(TypeID id,ColliderType colliderType, DirectX::SimpleMath::Vector3 size, DirectX::SimpleMath::Vector3 initialPos)
{
    m_typeID = id;
    m_colliderType = colliderType;
    m_boudingBox->Extents = size;
    m_initalePosition = initialPos;
    SetActive(true);
}

// �X�V����
void Collider::Update(float elapsedTime)
{
    UNREFERENCED_PARAMETER(elapsedTime);
    m_boudingBox->Center = GetOwner()->GetPosition() + m_initalePosition;
}

// �`�揈��
void Collider::Render()
{
	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	m_graphics->DrawPrimitivePositionColorBegin(view, projection, DirectX::SimpleMath::Matrix::Identity);
	DX::Draw(m_graphics->GetPrimitiveBatchPositionColor(), *m_boudingBox.get(), DirectX::Colors::Green);
	m_graphics->DrawPrimitivePositionColorEnd();
}


/// <summary>
/// �����蔻����Ƃ艟���߂��֐�
/// </summary>
/// <param name="object1"> �����߂����Ώ� </param>
/// <param name="object2"> ���肷�铖���蔻�� </param>
void Collider::CheckHit(GameObject* object1, GameObject* object2)
{
    using namespace DirectX::SimpleMath;

    DirectX::BoundingBox* a = object1->GetComponent<Collider>()->GetBoundingBox();
    DirectX::BoundingBox* b = object2->GetComponent<Collider>()->GetBoundingBox();

    if (object1->GetComponent<Collider>()->GetColliderType() != ColliderType::Collision) return;
    if (object2->GetComponent<Collider>()->GetColliderType() == ColliderType::Trigger)   return;

    // �Փˎ��A�a���`�������߂�����========================

    // AABB�p��min/max���v�Z����
    //Vector3 aMin = object1->GetPosition() - a->Extents;
    //Vector3 aMax = object1->GetPosition() + a->Extents;
    //Vector3 bMin = object2->GetPosition() - b->Extents;
    //Vector3 bMax = object2->GetPosition() + b->Extents;

    Vector3 aMin = a->Center - a->Extents;
    Vector3 aMax = a->Center + a->Extents;
    Vector3 bMin = b->Center - b->Extents;
    Vector3 bMax = b->Center + b->Extents;

    // �e���̍������v�Z����
    float dx1 = bMax.x - aMin.x;
    float dx2 = bMin.x - aMax.x;
    
    float dy1 = bMax.y - aMin.y;
    float dy2 = bMin.y - aMax.y;

    float dz1 = bMax.z - aMin.z;
    float dz2 = bMin.z - aMax.z;

    // �e���ɂ��āA��Βl�̏������������̂߂荞�ݗʂƂ���FAABB�̏d�Ȃ�����������肷��
    float dx = abs(dx1) < abs(dx2) ? dx1 : dx2;
    float dy = abs(dy1) < abs(dy2) ? dy1 : dy2;
    float dz = abs(dz1) < abs(dz2) ? dz1 : dz2;

    // �����߂��x�N�g��
    Vector3 pushBackVec = Vector3::Zero;
    Vector3 velocity = object1->GetVelocity();

    // �߂荞�݂���ԏ��������������߂�
    if (abs(dx) <= abs(dy) && abs(dx) <= abs(dz))
    {
        pushBackVec.x += dx;
        object1->SetVelocity({ 0, velocity.y, velocity.z });
    }

    if (abs(dz) <= abs(dx) && abs(dz) <= abs(dy))
    {
        pushBackVec.z += dz;
        object1->SetVelocity({ velocity.x,velocity.y,0 });
    }

    //if (abs(dy) <= abs(dx) && abs(dy) <= abs(dz))
    //{
    //    pushBackVec.y += dy;
    //    object1->SetVelocity({ velocity.x,0,velocity.z });
    //}

    // �����߂�
    object1->SetPosition(object1->GetPosition() + pushBackVec);
    Ray ray{ object1->GetPosition(), Vector3::Down };
    Ray reverseRay{ object1->GetPosition(),Vector3::Up };
    float distance = 0.0f;

    // �����ɓ����蔻�肪���邩
    if (ray.Intersects(*b, distance))
    {
        Vector3 hitPostion = Vector3{ ray.position + ray.direction * distance };

        if (object1->GetPosition().y - a->Extents.y < hitPostion.y)
        {
            // �����߂�
            object1->SetPosition(Vector3(object1->GetPosition().x, hitPostion.y + a->Extents.y, object1->GetPosition().z));
            object1->SetVelocity({ velocity.x,0,velocity.z });
            if(object1->GetComponent<Physics>()) object1->GetComponent<Physics>()->Reset();
        }
    }

    // �㑤�ɓ����蔻�肪���邩
    if (reverseRay.Intersects(*b, distance))
    {
        Vector3 hitPostion = Vector3{ reverseRay.position + reverseRay.direction * distance };

        if (object1->GetPosition().y - a->Extents.y < hitPostion.y)
        {
            // �����߂�
            object1->SetPosition(Vector3(object1->GetPosition().x, hitPostion.y - a->Extents.y, object1->GetPosition().z));
            object1->SetVelocity({ velocity.x,0,velocity.z });
        }
    }

}