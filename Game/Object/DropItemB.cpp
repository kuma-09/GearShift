#include "pch.h"
#include "DropItemB.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Physics.h"

DropItemB::DropItemB(IScene* scene)
{
	SetScene(scene);
	AddComponent<Physics>();
	AddComponent<Collider>();
	GetComponent<Collider>()->SetTypeID(Collider::TypeID::DropItemB);
	GetComponent<Collider>()->SetSize(DirectX::SimpleMath::Vector3::One * 2);
	AddComponent<ModelDraw>();
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetDropItemModel(),true);


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

DropItemB::~DropItemB()
{
	Finalize();
}

void DropItemB::Initialize()
{
	using namespace DirectX::SimpleMath;


	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItemB::Update(float elapsedTime)
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

void DropItemB::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow();
}

void DropItemB::Render()
{
	GetComponent<ModelDraw>()->Render();
}

void DropItemB::Finalize()
{
}

void DropItemB::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::Floor ||
		collider->GetTypeID() == Collider::Wall)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}
	if (collider->GetTypeID() == Collider::Player)
	{

	}
}