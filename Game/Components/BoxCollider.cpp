#include "pch.h"
#include "BoxCollider.h"
#include "Game/IScene.h"
#include "Game/GameObject.h"
#include "Framework/Microsoft/DebugDraw.h"
#include "Game/PlayScene.h"

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
    static_cast<PlayScene*>(GetOwner()->GetScene())->AddCollider(this);
}

/// <summary>
/// 当たり判定をとり押し戻す関数
/// </summary>
/// <param name="object1"> 押し戻される対象 </param>
/// <param name="object2"> 判定する当たり判定 </param>
void BoxCollider::CheckHit(GameObject* object1, GameObject* object2)
{
    using namespace DirectX::SimpleMath;

    DirectX::BoundingBox* a = object1->GetComponent<BoxCollider>()->GetBoundingBox();
    DirectX::BoundingBox* b = object2->GetComponent<BoxCollider>()->GetBoundingBox();

    // ヒットしていなければ終わり
    if (!a->Intersects(*b)) { return; }

    // 衝突時、ＢがＡを押し戻す処理========================

    // AABB用のmin/maxを計算する
    Vector3 aMin = a->Center - a->Extents;
    Vector3 aMax = a->Center + a->Extents;
    Vector3 bMin = b->Center - b->Extents;
    Vector3 bMax = b->Center + b->Extents;

    // 各軸の差分を計算する
    float dx1 = bMax.x - aMin.x;
    float dx2 = bMin.x - aMax.x;
    
    float dy1 = bMax.y - aMin.y;
    float dy2 = bMin.y - aMax.y;

    float dz1 = bMax.z - aMin.z;
    float dz2 = bMin.z - aMax.z;

    // 各軸について、絶対値の小さい方を軸のめり込み量とする：AABBの重なった部分を特定する
    float dx = abs(dx1) < abs(dx2) ? dx1 : dx2;
    float dy = abs(dy1) < abs(dy2) ? dy1 : dy2;
    float dz = abs(dz1) < abs(dz2) ? dz1 : dz2;

    // 押し戻しベクトル
    Vector3 pushBackVec = Vector3::Zero;
    Vector3 velocity = object1->GetVelocity();

    // めり込みが一番小さい軸を押し戻す
    if (abs(dx) <= abs(dy) && abs(dx) <= abs(dz))
    {
        pushBackVec.x += dx;
        object1->SetVelocity({ 0, velocity.y, velocity.z });
    }

    if (abs(dy) <= abs(dx) && abs(dy) <= abs(dz))
    {
        pushBackVec.y += dy;
        object1->SetVelocity({ velocity.x,0,velocity.z });
    }

    if (abs(dz) <= abs(dx) && abs(dz) <= abs(dy))
    {
        pushBackVec.z += dz;
        object1->SetVelocity({ velocity.x,velocity.y,0 });
    }


    // 押し戻す

    object1->SetPosition(object1->GetPosition() + pushBackVec);

    Matrix world = Matrix::Identity;
    world = Matrix::CreateScale(object1->GetScale());
    world *= Matrix::CreateFromQuaternion(object1->GetQuaternion());
    world *= Matrix::CreateTranslation(object1->GetPosition());

    object1->SetWorld(world);
    object1->GetComponent<BoxCollider>()->Update(0.0f);
    
}