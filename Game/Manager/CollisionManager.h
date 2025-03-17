#pragma once

class Collider;

/// <summary>
/// �R���C�_�[�R���|�[�l���g���Ǘ�����}�l�[�W���[
/// </summary>
class CollisionManager
{
public:
	// �R���C�_�[�R���|�[�l���g���}�l�[�W���[�ɒǉ�
	static void Add(Collider* component);
	// �X�V����
	static void Update();
	// �`�揈��
	static void Render();
	// �R���|�[�l���g���폜
	static void Remove(Collider* component);
private:
	// �����蔻������邩�����ƃR���C�_�[�̎�ނ��画��
	static bool DistanceAndTypeIDCheck(Collider* collider1, Collider* collider2);
private:
	// �R���|�[�l���g���܂Ƃ߂��z��
	static std::vector<Collider*> s_colliders;
};
