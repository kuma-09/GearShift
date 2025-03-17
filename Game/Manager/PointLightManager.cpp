#include "pch.h"
#include "PointLightManager.h"

std::vector<PointLight*> PointLightManager::s_pointLights;

// �|�C���g���C�g�R���|�[�l���g��ǉ�
void PointLightManager::Add(PointLight* component)
{
	s_pointLights.emplace_back(component);
}

// �X�V����
void PointLightManager::Update()
{
}

// �R���|�[�l���g���폜
void PointLightManager::Remove(PointLight* component)
{
	if (s_pointLights.empty()) return;
	s_pointLights.erase(std::remove(s_pointLights.begin(), s_pointLights.end(), component), s_pointLights.end());
}
