#include "pch.h"
#include "BoxCollider.h"
#include "Game/IScene.h"
#include "Game/GameObject.h"
#include "Framework/Microsoft/DebugDraw.h"

BoxCollider::BoxCollider()
{
	m_graphics = Graphics::GetInstance();
	m_boudingBox = std::make_unique<DirectX::BoundingBox>();
	m_boudingBox->Center = DirectX::SimpleMath::Vector3::Zero;
	m_boudingBox->Extents = DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.5f);
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::Initialize()
{

}

void BoxCollider::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	m_boudingBox->Center = GetOwner()->GetPosition();
}

void BoxCollider::Render()
{


	auto view       = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();

	m_graphics->DrawPrimitivePositionColorBegin(view, projection, DirectX::SimpleMath::Matrix::Identity);
	DX::Draw(m_graphics->GetPrimitiveBatchPositionColor(), *m_boudingBox.get(), DirectX::Colors::Green);
	m_graphics->DrawPrimitivePositionColorEnd();
}

void BoxCollider::Finalize()
{

}

void BoxCollider::SetSize(DirectX::SimpleMath::Vector3 size)
{
	m_boudingBox->Extents = size;
}

void BoxCollider::SetTypeID(TypeID id)
{
    m_typeID = id;
    GetOwner()->GetScene()->AddCollider(this);
}

void BoxCollider::CheckHit(GameObject* object1, GameObject* object2)
{
    using namespace DirectX::SimpleMath;

    DirectX::BoundingBox* a = object1->GetComponent<BoxCollider>().lock().get()->GetBoundingBox();
    DirectX::BoundingBox* b = object2->GetComponent<BoxCollider>().lock().get()->GetBoundingBox();

    // �q�b�g���Ă��Ȃ���ΏI���
    if (!a->Intersects(*b)) { return; }

    // �Փˎ��A�a���`�������߂�����========================

    // AABB�p��min/max���v�Z����
    Vector3 aMin = a->Center - a->Extents;
    // �@aMax,bMin,bMax���v�Z����

    // �e���̍������v�Z����
    float dx1 = (b->Center.x + b->Extents.x) - (a->Center.x - a->Extents.x);
    float dx2 = (b->Center.x - b->Extents.x) - (a->Center.x + a->Extents.x);
    // �Ady1,dy2,dz1,dz2���v�Z����
    float dy1 = (b->Center.y + b->Extents.y) - (a->Center.y - a->Extents.y);
    float dy2 = (b->Center.y - b->Extents.y) - (a->Center.y + a->Extents.y);

    float dz1 = (b->Center.z + b->Extents.z) - (a->Center.z - a->Extents.z);
    float dz2 = (b->Center.z - b->Extents.z) - (a->Center.z + a->Extents.z);

    // �e���ɂ��āA��Βl�̏������������̂߂荞�ݗʂƂ���FAABB�̏d�Ȃ�����������肷��
    float dx = abs(dx1) < abs(dx2) ? dx1 : dx2;
    // �Bdy,dz���v�Z����
    float dy = abs(dy1) < abs(dy2) ? dy1 : dy2;
    float dz = abs(dz1) < abs(dz2) ? dz1 : dz2;

    // �����߂��x�N�g��
    Vector3 pushBackVec = Vector3::Zero;

    // �߂荞�݂���ԏ��������������߂��i�w�����ŏ��̏ꍇ�j
    if (abs(dx) <= abs(dy) && abs(dx) <= abs(dz))
    {
        pushBackVec.x += dx;
    }
    // �C�x���A�y�����ŏ��̏ꍇ���v�Z����
    if (abs(dy) <= abs(dx) && abs(dy) <= abs(dz))
    {
        pushBackVec.y += dy;
    }

    if (abs(dz) <= abs(dx) && abs(dz) <= abs(dy))
    {
        pushBackVec.z += dz;
    }

    // �`�������߂��i�`�̏Փ˔�������X�V����j
    // �D
    object1->SetPosition(object1->GetPosition() + pushBackVec);
}