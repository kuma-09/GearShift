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

    // ヒットしていなければ終わり
    if (!a->Intersects(*b)) { return; }

    // 衝突時、ＢがＡを押し戻す処理========================

    // AABB用のmin/maxを計算する
    Vector3 aMin = a->Center - a->Extents;
    // ①aMax,bMin,bMaxを計算する

    // 各軸の差分を計算する
    float dx1 = (b->Center.x + b->Extents.x) - (a->Center.x - a->Extents.x);
    float dx2 = (b->Center.x - b->Extents.x) - (a->Center.x + a->Extents.x);
    // ②dy1,dy2,dz1,dz2を計算する
    float dy1 = (b->Center.y + b->Extents.y) - (a->Center.y - a->Extents.y);
    float dy2 = (b->Center.y - b->Extents.y) - (a->Center.y + a->Extents.y);

    float dz1 = (b->Center.z + b->Extents.z) - (a->Center.z - a->Extents.z);
    float dz2 = (b->Center.z - b->Extents.z) - (a->Center.z + a->Extents.z);

    // 各軸について、絶対値の小さい方を軸のめり込み量とする：AABBの重なった部分を特定する
    float dx = abs(dx1) < abs(dx2) ? dx1 : dx2;
    // ③dy,dzを計算する
    float dy = abs(dy1) < abs(dy2) ? dy1 : dy2;
    float dz = abs(dz1) < abs(dz2) ? dz1 : dz2;

    // 押し戻しベクトル
    Vector3 pushBackVec = Vector3::Zero;

    // めり込みが一番小さい軸を押し戻す（Ｘ軸が最小の場合）
    if (abs(dx) <= abs(dy) && abs(dx) <= abs(dz))
    {
        pushBackVec.x += dx;
    }
    // ④Ｙ軸、Ｚ軸が最小の場合を計算する
    if (abs(dy) <= abs(dx) && abs(dy) <= abs(dz))
    {
        pushBackVec.y += dy;
    }

    if (abs(dz) <= abs(dx) && abs(dz) <= abs(dy))
    {
        pushBackVec.z += dz;
    }

    // Ａを押し戻す（Ａの衝突判定情報も更新する）
    // ⑤
    object1->SetPosition(object1->GetPosition() + pushBackVec);
}