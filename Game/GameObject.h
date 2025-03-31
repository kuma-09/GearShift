#pragma once
#include "pch.h"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <array>
#include "Components/IComponent.h"
#include "IScene.h"

namespace ObjectType
{
	enum TypeID
	{
		None,
		Player,
		PlayerBullet,
		PlayerSword,
		Enemy,
		EnemyBullet,
		Wall,
		DropItem,
		Floor
	};
}

class GameObject
{
public:
	GameObject();
	~GameObject() = default;

	// ���W
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// �x�N�g��
	void SetVelocity(DirectX::SimpleMath::Vector3 vel) { m_velocity = vel; }
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }

	// �p�x
	void SetRotation(DirectX::SimpleMath::Vector3 rot) { m_rotation = rot; }
	DirectX::SimpleMath::Vector3 GetRotation() { return m_rotation; }

	// �p�x(Quaternion)
	void SetQuaternion(DirectX::SimpleMath::Quaternion qua) { m_quaternion = qua; }
	DirectX::SimpleMath::Quaternion GetQuaternion() { return m_quaternion; }

	// �傫��
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }
	DirectX::SimpleMath::Vector3 GetScale() { return m_scale; }

	// ���[���h�s��
	void SetWorld(DirectX::SimpleMath::Matrix world) { m_world = world; }
	DirectX::SimpleMath::Matrix GetWorld() { return m_world; }

	// �R���|�[�l���g��ǉ�
	template<typename CompType>
	void AddComponent()
	{
		std::unique_ptr<CompType> newComp = std::make_unique<CompType>();
		m_umComponents[typeid(CompType)] = std::move(newComp);
		m_umComponents[typeid(CompType)]->SetOwner(this);
	}
	// �R���|�[�l���g���擾
	template<typename CompType>
	CompType* GetComponent()
	{
		auto it = m_umComponents.find(typeid(CompType));
		if (it != m_umComponents.end())
		{
			return static_cast<CompType*>(it->second.get());
		}
		return nullptr;
	}

	// �q�I�u�W�F�N�g��ǉ�
	template<typename ObjectType>
	void SetChild(std::string name)
	{
		std::unique_ptr<ObjectType> newObject = std::make_unique<ObjectType>();
		m_umChildObjects[name] = std::move(newObject);
		m_umChildObjects[name]->SetOwner(this);
	}
	// �q�I�u�W�F�N�g���擾
	template<typename ObjectType>
	ObjectType* GetChild(std::string name)
	{
		auto it = m_umChildObjects.find(name);
		if (it != m_umChildObjects.end())
		{
			return static_cast<ObjectType*>(it->second.get());
		}
		return nullptr;
	}


	// �R���|�[�l���g���X�V
	void ComponentsUpdate(float elapsedTime);
	// �q�I�u�W�F�N�g���X�V
	void ChildObjectsUpdate(float elapsedTime);

	void SetOwner(GameObject* owner) { m_owner = owner; }
	GameObject* GetOwner() { return m_owner; }

	void SetScene(IScene* scene) { m_scene = scene; }
	IScene* GetScene() { return m_scene; }

	void SetType(ObjectType::TypeID type) { m_typeID = type; }
	const ObjectType::TypeID GetType() { return m_typeID; }
	
public:
	virtual void Initialize() {};
	virtual void Update(float elapsedTime) { UNREFERENCED_PARAMETER(elapsedTime); }
	virtual void Collision(Collider* collider) { UNREFERENCED_PARAMETER(collider); }
private:
	IScene* m_scene = nullptr;
	ObjectType::TypeID m_typeID;
	GameObject* m_owner;

	// ���W�Ȃ�
	DirectX::SimpleMath::Vector3	m_position = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3	m_rotation = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3    m_velocity = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Quaternion m_quaternion = DirectX::SimpleMath::Quaternion::Identity;
	DirectX::SimpleMath::Vector3	m_scale = DirectX::SimpleMath::Vector3::One;
	DirectX::SimpleMath::Matrix     m_world = DirectX::SimpleMath::Matrix::Identity;

	// �R���|�[�l���g�z��
	std::unordered_map<std::type_index, std::unique_ptr<IComponent>> m_umComponents;

	std::unordered_map<std::string, std::unique_ptr<GameObject>> m_umChildObjects;
};
