#include "pch.h"
#include "MissileLauncher.h"
#include "Framework/Resources.h"
#include "Framework/Audio.h"
#include "Game/Components/ModelDraw.h"

#include "Game/Object/Bullet/HomingBullet.h"

MissileLauncher::MissileLauncher(GameObject* owner)
{
	m_owner = owner;
	//AddComponent<ModelDraw>();
}

MissileLauncher::~MissileLauncher()
{
}

void MissileLauncher::Initialize()
{
	//GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetGunModel());

	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_bullet.emplace_back(std::make_unique<HomingBullet>(m_owner->GetScene(), Collider::PlayerBullet));
		m_bullet.back()->Initialize(this);
	}

	m_missileInterval = 0;
}

void MissileLauncher::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_missileInterval += elapsedTime;

	ComponentsUpdate(elapsedTime);

	for (auto& bullet : m_bullet)
	{
		bullet->Update(elapsedTime);
	}

	SetQuaternion(Quaternion::CreateFromYawPitchRoll(0, DirectX::XMConvertToRadians(-90), 0) * m_owner->GetQuaternion());
	SetPosition(m_owner->GetPosition() + Vector3::Transform(Vector3(0.75f, 1.0f, 0.25f), GetQuaternion()));
	//SetPosition(m_owner->GetPosition());

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void MissileLauncher::Shot(GameObject* target)
{
	if (m_missileInterval < INTERVAL || !target)
	{
		return;
	}
	int usedCount = 0;
	m_missileInterval = 0;
	for (auto& bullet : m_bullet)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(target);
			Audio::GetInstance()->PlaySoundSE_Rocket();
			break;
		}
	}
}

void MissileLauncher::Reload()
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_bullet[i]->Initialize(this);
	}
}

size_t MissileLauncher::GetMagazineSize()
{
	int value = 0;
	for (auto& bullet : m_bullet)
	{
		if (bullet->GetState() == Bullet::UNUSED)
		{
			value++;
		}
	}
	return value;
}
