#include "pch.h"
#include "RenderManager.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Particle/Emitter.h"
#include "Game/Components/Trail.h"
#include "Game/Shader/ShadowMap.h"

std::vector<ModelDraw*> RenderManager::s_modelDraws;
std::vector<Emitter*> RenderManager::s_emitters;
std::vector<Trail*>  RenderManager::s_trail;

void RenderManager::Add(ModelDraw* component)
{
	s_modelDraws.emplace_back(component);
}

void RenderManager::Add(Emitter* component)
{
	s_emitters.emplace_back(component);
}

void RenderManager::Add(Trail* component)
{
	s_trail.emplace_back(component);
}

void RenderManager::CreateShadowMap()
{
	ShadowMap::BeginDepth();
	for (auto& modelDraw: s_modelDraws)
	{
		modelDraw->CreateShadow();
	}
	ShadowMap::EndDepth();
}

void RenderManager::RenderObjects()
{
	for (auto& modelDraw : s_modelDraws)
	{
		modelDraw->Render();
	}
}

void RenderManager::RenderParticle()
{
	for (auto& emitter : s_emitters)
	{
		emitter->Render();
	}
	for (auto& trail : s_trail)
	{
		trail->Render();
	}
}

void RenderManager::Remove(ModelDraw* component)
{
	s_modelDraws.erase(std::remove(s_modelDraws.begin(), s_modelDraws.end(), component), s_modelDraws.end());
}

void RenderManager::Remove(Emitter* component)
{
	s_emitters.erase(std::remove(s_emitters.begin(), s_emitters.end(), component), s_emitters.end());
}

void RenderManager::Remove(Trail* component)
{
	s_trail.erase(std::remove(s_trail.begin(), s_trail.end(), component), s_trail.end());
}

void RenderManager::Clear()
{
	s_modelDraws.clear();
	s_emitters.clear();
	s_trail.clear();
}
