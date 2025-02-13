#include "pch.h"	
#include "Gun.h"
#include "Framework/Resources.h"
#include "Framework/Audio.h"
#include "Game/Components/ModelDraw.h"

#include "Game/Object/Bullet/NormalBullet.h"

Gun::Gun(GameObject* owner)
{
	m_owner = owner;
	AddComponent<ModelDraw>();
}

Gun::~Gun()
{
}

void Gun::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Gun));

	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_defaultBullet.emplace_back(std::make_unique<NormalBullet>(m_owner->GetScene(), Collider::PlayerBullet));
		m_defaultBullet.back()->Initialize(this);
	}
	m_bulletInterval = 0;
	m_nowReloadTime = 0;
	m_isReload = false;
}

void Gun::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_bulletInterval += elapsedTime;

	if (m_isReload)
	{
		m_nowReloadTime += elapsedTime;
		if (m_nowReloadTime > RELOAD_TIME)
		{
			m_isReload = false;
			m_nowReloadTime = 0;
			for (int i = 0; i < MAX_BULLET_COUNT; i++)
			{
				m_defaultBullet[i]->Initialize(this);
			}
		}
	}

	ComponentsUpdate(elapsedTime);

	for (auto& bullet: m_defaultBullet)
	{
		bullet->Update(elapsedTime);
	}

	SetQuaternion(Quaternion::CreateFromYawPitchRoll(0, DirectX::XMConvertToRadians(-90),0) * m_owner->GetQuaternion());
	SetPosition(m_owner->GetPosition() + Vector3::Transform(Vector3(0.75f,1.0f,0.25f), GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Gun::Shot(GameObject* target)
{
	if (m_bulletInterval < INTERVAL) return;
	if (!target)					 return;
	if (m_isReload)					 return;

	int usedCount = 0;
	m_bulletInterval = 0;
	for (auto& bullet : m_defaultBullet)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(target);
			Audio::GetInstance()->PlaySoundSE_Rocket();
			break;
		}
	}
}

void Gun::Reload()
{
	m_isReload = true;
}

size_t Gun::GetMagazineSize()
{
	int value = 0;
	for (auto& bullet : m_defaultBullet)
	{
		if (bullet->GetState() == Bullet::UNUSED)
		{
			value++;
		}
	}
	return value;
}

float Gun::GetNowReloadTime()
{
	if (m_isReload)
	{
		return m_nowReloadTime;
	}		
	return 0.0f;
}
