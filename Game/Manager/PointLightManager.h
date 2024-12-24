#pragma once

#include "Game/Components/PointLight.h"

class PointLightManager
{
public:
	static void Add(PointLight* component);
	static void Update();
	static void Remove(PointLight* component);
	static std::vector<PointLight*> GetPointLights() { return s_pointLights; }
private:
	static std::vector<PointLight*> s_pointLights;
};
