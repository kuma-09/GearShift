#include "pch.h"
#include "PointLightManager.h"

std::vector<PointLight*> PointLightManager::s_pointLights;

void PointLightManager::Add(PointLight* component)
{
	s_pointLights.emplace_back(component);
}

void PointLightManager::Update()
{
}

void PointLightManager::Remove(PointLight* component)
{
	if (s_pointLights.empty()) return;
	s_pointLights.erase(std::remove(s_pointLights.begin(), s_pointLights.end(), component), s_pointLights.end());
}
