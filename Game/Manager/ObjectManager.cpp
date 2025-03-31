#include "pch.h"	
#include "ObjectManager.h"

std::vector<std::shared_ptr<GameObject>> ObjectManager::s_gameObjects;
std::vector<GameObject*> ObjectManager::s_deleteObjects;

// �Q�[���I�u�W�F�N�g���}�l�[�W���[�ɒǉ�����
std::weak_ptr<GameObject> ObjectManager::Add(std::shared_ptr<GameObject> object, DirectX::SimpleMath::Vector3 pos, ObjectType::TypeID type)
{
	s_gameObjects.emplace_back(object);
	s_gameObjects.back()->Initialize();
	s_gameObjects.back()->SetType(type);
	s_gameObjects.back()->SetPosition(pos);
	s_gameObjects.back()->SetWorld(DirectX::SimpleMath::Matrix::CreateTranslation(pos));
	return std::static_pointer_cast<GameObject>(object);
}

// �X�V����
void ObjectManager::Update(float elapsedTime)
{
	for (auto& object : s_gameObjects)
	{
		object->Update(elapsedTime);
	}
}

// �Q�[���I�u�W�F�N�g���폜�\��z��ɒǉ�
void ObjectManager::Remove(GameObject* object)
{
	s_deleteObjects.emplace_back(object);
}

// �폜�\��I�u�W�F�N�g���܂Ƃ߂č폜
void ObjectManager::Delete()
{
	for (auto& deleteObject : s_deleteObjects)
	{
		// �폜�Ώۂ�����
		auto it = std::remove_if(
			s_gameObjects.begin(),
			s_gameObjects.end(),
			[deleteObject](const std::shared_ptr<GameObject>& obj) {
				return obj.get() == deleteObject; // �����|�C���^���r
			});

		// �폜�Ώۂ����X�g����폜
		s_gameObjects.erase(it, s_gameObjects.end());
	}
	// �폜�\��z����N���A
	s_deleteObjects.clear();
}

// ���ׂẴQ�[���I�u�W�F�N�g���폜
void ObjectManager::Clear()
{
	s_gameObjects.clear();
}

// �^�C�v���ƂɃI�u�W�F�N�g���擾
std::vector<std::weak_ptr<GameObject>> ObjectManager::GetTypeObjects(ObjectType::TypeID type)
{
	std::vector<std::weak_ptr<GameObject>> results;

	for (auto& object : s_gameObjects)
	{
		if (object->GetType() == type)
		{
			results.emplace_back(std::static_pointer_cast<GameObject>(object));
		}
	}
	return results;
}
