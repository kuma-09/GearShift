#pragma once
#include "pch.h"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <array>
#include "Components/IComponent.h"
#include "IScene.h"

class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() = default;

	// ���W
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// �p�x
	void SetRotation(DirectX::SimpleMath::Vector3 rot) { m_rotation = rot; }
	DirectX::SimpleMath::Vector3 GetRotation() { return m_rotation; }

	// �p�x(Quaternion)
	void SetQuaternion(DirectX::SimpleMath::Quaternion qua) { m_quaternion = qua; }
	DirectX::SimpleMath::Quaternion GetQuaternion() { return m_quaternion; }

	// �傫��
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }
	DirectX::SimpleMath::Vector3 GetScale() { return m_scale; }

	void SetWorld(DirectX::SimpleMath::Matrix world) { m_world = world; }
	DirectX::SimpleMath::Matrix GetWorld() { return m_world; }

	// �R���|�[�l���g��ǉ�
	template<typename CompType>
	std::weak_ptr<CompType> AddComponent()
	{
		std::shared_ptr<CompType> newComp = std::make_shared<CompType>();
		m_umComponents[typeid(CompType)] = newComp;
		newComp->SetOwner(this);
		return newComp;
	}

	// �R���|�[�l���g���擾
	template<typename CompType>
	std::weak_ptr<CompType> GetComponent()
	{
		return std::static_pointer_cast<CompType>(m_umComponents[typeid(CompType)]);
	}

	// �R���|�[�l���g���X�V
	void ComponentsUpdate(float elapsedTime)
	{
		for (auto&& spComp : m_umComponents)
		{
			spComp.second->Update(elapsedTime);
		}
	}


public:
	
	virtual void Update(float elapsedTime) = 0;


private:
	DirectX::SimpleMath::Vector3	m_position = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3	m_rotation = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Quaternion m_quaternion = DirectX::SimpleMath::Quaternion::Identity;
	DirectX::SimpleMath::Vector3	m_scale = DirectX::SimpleMath::Vector3::One;
	DirectX::SimpleMath::Matrix     m_world = DirectX::SimpleMath::Matrix::Identity;

	std::unordered_map<std::type_index, std::shared_ptr<IComponent>> m_umComponents;
};
