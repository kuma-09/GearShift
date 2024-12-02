#include "pch.h"
#include "DropItem.h"
#include "Game/Components/Physics.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"

DropItem::DropItem(IScene* scene, std::unique_ptr<Part> part)
{

	SetScene(scene);
	AddComponent<Physics>();
	AddComponent<Collider>();
	GetComponent<Collider>()->SetTypeID(Collider::TypeID::DropItem);
	GetComponent<Collider>()->SetSize(DirectX::SimpleMath::Vector3::One * 2);
	AddComponent<ModelDraw>();
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetDropItemModel());

	m_part = std::move(part);

	using namespace DirectX;

	SetScale({ m_dropItemModelSize,m_dropItemModelSize,m_dropItemModelSize });

	// ���f���̃G�t�F�N�g�����X�V����
	Resources::GetInstance()->GetDropItemModel()->UpdateEffects([](IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// ���f����������������
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

}

DropItem::~DropItem()
{
	Finalize();
}

void DropItem::Initialize()
{
	using namespace DirectX::SimpleMath;



	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItem::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetQuaternion(GetQuaternion() * Quaternion::CreateFromYawPitchRoll({ 0,1 * 3.14 / 180,0 }));
	SetPosition(GetPosition() + GetVelocity() * elapsedTime);

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItem::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow(GetWorld());
}

void DropItem::Render()
{
	GetComponent<ModelDraw>()->Render(GetWorld(),true);
	if (m_isHit)
	{
		//m_menu->Render();
	}
}

void DropItem::Finalize()
{
}

void DropItem::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::Floor ||
		collider->GetTypeID() == Collider::Wall)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}
}
