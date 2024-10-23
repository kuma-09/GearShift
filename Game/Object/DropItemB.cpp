#include "pch.h"
#include "DropItemB.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

DropItemB::DropItemB(IScene* scene, std::vector<std::unique_ptr<Bullet>> wepons)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::DropItem);
	AddComponent<ModelDraw>();
	GetComponent<ModelDraw>()->Initialize(ModelDraw::DropItem);

	m_menu = std::make_unique<UI>(L"Resources/Textures/window.png",DirectX::SimpleMath::Vector2::Zero);
	m_bullet = std::move(wepons);

	using namespace DirectX;

	// モデルのエフェクト情報を更新する
	Resources::GetInstance()->GetDropItemModel()->UpdateEffects([](IEffect* effect)
		{
			// ベーシックエフェクトを設定する
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// モデルを自発光させる
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

	SetScale({ m_dropItemModelSize,m_dropItemModelSize,m_dropItemModelSize});

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

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void DropItemB::Render()
{
	GetComponent<ModelDraw>()->Render(true,DirectX::Colors::Red);
}

void DropItemB::Finalize()
{
}