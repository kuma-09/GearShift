#include "pch.h"
#include "Sword.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Trail.h"
#include "Game/PlayScene.h"
#include "Framework/Audio.h"

#include "Game/System/HitStop.h"
#include "Framework/Easing.h"

// コンストラクタ
Blade::Blade(Scene* scene, Collider::TypeID id)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<Trail>();
	GetComponent<Collider>()->Initialize(id,Collider::Fixed, { 0.5f,0.5f,0.5f});
	GetComponent<Trail>()->Initialize(L"Resources/Textures/white.png", 10,DirectX::Colors::CadetBlue);
	SetScale(SIZE);
	SetState(BladeState::UNUSED);
}

// デストラクタ
Blade::~Blade()
{
}

// 初期化処理
void Blade::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	m_owner = object;
	Vector3 velocity = Vector3::Zero;
	SetPosition(m_owner->GetPosition());
	SetQuaternion(m_owner->GetQuaternion());
	GetComponent<Trail>()->ClearBuffer();
	SetVelocity(Vector3::Zero);
	SetState(BladeState::USING);
	m_rotate = 0;
	m_isHit = false;
	GetComponent<Collider>()->SetActive(true);	
}

// 弾を発射
void Blade::Shot(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(object->GetPosition());
	SetQuaternion(object->GetQuaternion());

	velocity += Vector3::Transform(Vector3::Forward, GetQuaternion());

	SetVelocity(velocity);
	SetState(BladeState::USING);
}

// 何かに当たった時の処理
void Blade::Hit()
{
	using namespace DirectX::SimpleMath;

	if (GetState()!=USED)
	{
		SetState(USED);
		Audio::GetInstance()->PlaySoundSE_Slash();
		HitStop::SetStopTime(0.055f);
		GetComponent<Collider>()->SetActive(false);
	}
}

// 更新処理
void Blade::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;


	float rotate = Easing::InQuart(m_rotate, 0.25f);

	// 
	if (GetState() == USED)
	{
		m_rotate += elapsedTime;
		if (rotate >= 1)
		{
			GetComponent<Trail>()->ClearBuffer();
			m_isHit = true;
		}
	}

	SetPosition(m_owner->GetPosition() + Vector3::Transform({ 0,0,-1 }, m_owner->GetQuaternion()));
	ComponentsUpdate(elapsedTime);
	SetPosition(m_owner->GetPosition());
	SetQuaternion(m_owner->GetQuaternion());

	Matrix world = Matrix::Identity;
	// ５頂点トレイルを生成
	for (int i = 3; i < 5; i++)
	{
		world = Matrix::CreateScale(GetScale());
		world *= Matrix::CreateTranslation(Vector3{ 0,0,i * -1.5f });
		if (GetState() == USED)
		{
			world *= Matrix::CreateRotationY(XMConvertToRadians(100 - rotate * 200));
			world *= Matrix::CreateRotationZ(XMConvertToRadians(-45));
		}
		world *= Matrix::CreateFromQuaternion(GetQuaternion());
		world *= Matrix::CreateTranslation(GetPosition());
	}
	GetComponent<Collider>()->GetBoundingBox()->Center = { world._41, world._42, world._43 };
	GetComponent<Trail>()->SetPos(GetPosition(), { world._41, world._42, world._43 });

}

// 当たり判定の処理
void Blade::Collision(Collider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}
