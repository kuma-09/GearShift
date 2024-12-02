/*
	@file	IScene.h
	@brief	シーンのインターフェイスクラス
*/
#pragma once
#include "Game/Components/Collider.h"

class Game;

class IScene
{

public:
	virtual ~IScene() = default;

	virtual void Initialize(Game* game) = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void RenderUI() = 0;

	virtual void Finalize() = 0;

	// ボックスコライダーを追加
	void AddCollider(Collider* collider)
	{
		m_pCollider.emplace_back(collider);
	}

	// コライダーを削除する
	void RemoveCollider(Collider* collider)
	{
		int it = 0;
		for (auto Colliders : m_pCollider)
		{
			if (Colliders == collider)
			{
				break;
			}
			it++;
		}
		if (m_pCollider.size() > it)
		{
			m_pCollider.erase(m_pCollider.begin() + it);
		}
	}

	// ボックスコライダーの配列を取得
	std::vector<Collider*> GetColliders() { return m_pCollider; }

	void SetGame(Game* game) { m_game = game; }
	Game* GetGame() { return m_game; }

private:

	Game* m_game;
	// ボックスコライダーをまとめた配列
	std::vector<Collider*> m_pCollider;

};
