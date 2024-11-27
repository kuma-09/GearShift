/*
	@file	IScene.h
	@brief	シーンのインターフェイスクラス
*/
#pragma once
#include "Game/Components/BoxCollider.h"

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
	void AddCollider(BoxCollider* collider)
	{
		m_pBoxCollider.emplace_back(collider);
	}

	// コライダーを削除する
	void RemoveCollider(BoxCollider* collider)
	{
		int it = 0;
		for (auto boxColliders : m_pBoxCollider)
		{
			if (boxColliders == collider)
			{
				break;
			}
			it++;
		}
		if (m_pBoxCollider.size() > it)
		{
			m_pBoxCollider.erase(m_pBoxCollider.begin() + it);
		}
	}

	// ボックスコライダーの配列を取得
	std::vector<BoxCollider*> GetColliders() { return m_pBoxCollider; }

	void SetGame(Game* game) { m_game = game; }
	Game* GetGame() { return m_game; }

private:

	Game* m_game;
	// ボックスコライダーをまとめた配列
	std::vector<BoxCollider*> m_pBoxCollider;

};
