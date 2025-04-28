#pragma once
#include "Component.h"
#include "Framework/Graphics.h"
#include "Game/Manager/CollisionManager.h"

/// <summary>
/// �����蔻��R���|�[�l���g�N���X
/// </summary>
class Collider : public Component
{
public:

	// �����蔻�蔻�ʗp�^�O
	enum TypeID
	{
		Player,
		PlayerBullet,
		PlayerSword,
		Enemy,
		EnemyBullet,
		Wall,
		DropItem,
		DropItemB,
		Floor
	};

	// �����蔻��̏����̎��
	enum ColliderType
	{
		Fixed,
		Collision,
		Trigger
	};

	// �R���X�g���N�^
	Collider();
	// �f�X�g���N�^
	~Collider();

	// ����������
	void Initialize(TypeID id,ColliderType colliderType,
		DirectX::SimpleMath::Vector3 size = DirectX::SimpleMath::Vector3::One,
		DirectX::SimpleMath::Vector3 initialPos = DirectX::SimpleMath::Vector3::Zero);
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();

	// �o�E���f�B���O�{�b�N�X���擾
	DirectX::BoundingBox* GetBoundingBox() { return m_boudingBox.get(); }
	// �^�OID���擾
	TypeID GetTypeID() { return m_typeID; }
	// �����蔻��̎�ނ��擾
	ColliderType GetColliderType() { return m_colliderType; }
	// �����蔻����I��/�I�t
	void SetActive(bool isActive) { m_isActive = isActive; }
	// Active�ȓ����蔻�肩�擾
	bool GetActive() { return m_isActive; }

	// �����߂�����
	static void CheckHit(GameObject* object1, GameObject* object2);

private:
	Graphics* m_graphics;
	TypeID m_typeID;
	ColliderType m_colliderType;
	std::unique_ptr<DirectX::BoundingBox> m_boudingBox;
	DirectX::SimpleMath::Vector3 m_initalePosition;
	bool m_isActive;
};

