#pragma once

#include "Game/Components/PointLight.h"

/// <summary>
/// �|�C���g���C�g�R���|�[�l���g���Ǘ�����}�l�[�W���[
/// </summary>
class PointLightManager
{
public:
	// �|�C���g���C�g�R���|�[�l���g��ǉ�
	static void Add(PointLight* component);
	// �X�V����
	static void Update();
	// �R���|�[�l���g���폜
	static void Remove(PointLight* component);
	// �|�C���g���C�g�̔z����擾
	static std::vector<PointLight*> GetPointLights() { return s_pointLights; }
private:
	// �|�C���g���C�g�z��
	static std::vector<PointLight*> s_pointLights;
};
