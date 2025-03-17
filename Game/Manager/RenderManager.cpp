#include "pch.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Trail.h"
#include "Game/Particle/Emitter.h"
#include "Game/Shader/ShadowMap.h"

std::vector<ModelDraw*> RenderManager::s_modelDraws;
std::vector<Emitter*> RenderManager::s_emitters;
std::vector<Trail*>  RenderManager::s_trail;

// コンポーネントを追加
void RenderManager::Add(ModelDraw* component)
{
	s_modelDraws.emplace_back(component);
}

// コンポーネントを追加
void RenderManager::Add(Emitter* component)
{
	s_emitters.emplace_back(component);
}

// コンポーネントを追加
void RenderManager::Add(Trail* component)
{
	s_trail.emplace_back(component);
}

// シャドウマップを作成
void RenderManager::CreateShadowMap()
{
	// カスケードシャドウマップ用に複数枚に描画
	for (int i = 0; i < SHADOWMAP_MAX; i++)
	{
		ShadowMap::BeginDepth(i);
		for (auto& modelDraw : s_modelDraws)
		{
			modelDraw->CreateShadow();
		}
		ShadowMap::EndDepth();
	}
}

// モデルを描画
void RenderManager::RenderObjects()
{
	for (auto& modelDraw : s_modelDraws)
	{
		modelDraw->Render();
	}

	// 当たり判定デバッグ用
	#ifdef _DEBUG
	CollisionManager::Render();
	#endif // _DEBUG

}

// パーティクルを描画
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

// コンポーネントを削除
void RenderManager::Remove(ModelDraw* component)
{
	if (s_modelDraws.empty()) return;
	s_modelDraws.erase(std::remove(s_modelDraws.begin(), s_modelDraws.end(), component), s_modelDraws.end());
}

// コンポーネントを削除
void RenderManager::Remove(Emitter* component)
{
	if (s_emitters.empty()) return;
	s_emitters.erase(std::remove(s_emitters.begin(), s_emitters.end(), component), s_emitters.end());
}

// コンポーネントを削除
void RenderManager::Remove(Trail* component)
{
	if (s_trail.empty()) return;
	s_trail.erase(std::remove(s_trail.begin(), s_trail.end(), component), s_trail.end());
}

// それぞれのコンポーネントをすべてクリア
void RenderManager::Clear()
{
	s_modelDraws.clear();
	s_emitters.clear();
	s_trail.clear();
}
