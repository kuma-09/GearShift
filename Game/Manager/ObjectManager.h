#pragma once

#include "Game/GameObject.h"

/// <summary>
/// �Q�[���I�u�W�F�N�g���Ǘ�����}�l�[�W���[
/// </summary>
class ObjectManager
{
public:
	// �Q�[���I�u�W�F�N�g���}�l�[�W���[�ɒǉ�����
	static std::weak_ptr<GameObject> Add(std::shared_ptr<GameObject> object,DirectX::SimpleMath::Vector3 pos,ObjectType::TypeID type = ObjectType::None);
	// �X�V����
	static void Update(float elapsedTime);
	// �Q�[���I�u�W�F�N�g���폜�\��z��ɒǉ�
	static void Remove(GameObject* object);
	// �폜�\��I�u�W�F�N�g���܂Ƃ߂č폜
	static void Delete();
	// ���ׂẴQ�[���I�u�W�F�N�g���폜
	static void Clear();
	// �^�C�v���ƂɃI�u�W�F�N�g���擾
	static std::vector<std::weak_ptr<GameObject>> GetTypeObjects(ObjectType::TypeID type);
private:
	// �Q�[���I�u�W�F�N�g�̔z��
	static std::vector<std::shared_ptr<GameObject>> s_gameObjects;
	// �폜�\��̃Q�[���I�u�W�F�N�g
	static std::vector<GameObject*> s_deleteObjects;
};
