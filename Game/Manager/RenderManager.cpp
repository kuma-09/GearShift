#include "pch.h"
#include "RenderManager.h"
#include "Game/GameObject.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Particle/Emitter.h"
#include "Game/Components/Trail.h"
#include "Game/Shader/ShadowMap.h"

#include "CollisionManager.h"

Graphics* RenderManager::s_graphics = Graphics::GetInstance();
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
	for (int i = 0; i < 4; i++)
	{
		ShadowMap::BeginDepth(i);
		for (auto& modelDraw : s_modelDraws)
		{
			modelDraw->CreateShadow();
		}
		ShadowMap::EndDepth();
	}
}

void RenderManager::RenderObjects()
{
	for (auto& modelDraw : s_modelDraws)
	{
		modelDraw->Render();
	}

	// 当たり判定デバッグ用
	// CollisionManager::Render();

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
	if (s_modelDraws.empty()) return;
	s_modelDraws.erase(std::remove(s_modelDraws.begin(), s_modelDraws.end(), component), s_modelDraws.end());
}

void RenderManager::Remove(Emitter* component)
{
	if (s_emitters.empty()) return;
	s_emitters.erase(std::remove(s_emitters.begin(), s_emitters.end(), component), s_emitters.end());
}

void RenderManager::Remove(Trail* component)
{
	if (s_trail.empty()) return;
	s_trail.erase(std::remove(s_trail.begin(), s_trail.end(), component), s_trail.end());
}

void RenderManager::Clear()
{
	s_modelDraws.clear();
	s_emitters.clear();
	s_trail.clear();
}
