/*
	@file	IScene.h
	@brief	シーンのインターフェイスクラス
*/
#pragma once
#include "Game/Components/BoxCollider.h"

class IScene
{

public:
	virtual ~IScene() = default;

	virtual void Initialize() = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;

	// コンポーネントを追加
	void AddCollider(BoxCollider* collider)
	{
		m_pBoxCollider.push_back(collider);
	}
	
	void RemoveCollider(BoxCollider* collider)
	{
		int it = 0;
		for (auto boxColliders: m_pBoxCollider)
		{
			it++;
			if (boxColliders == collider)
			{
				break;
			}
		}

		m_pBoxCollider.erase(m_pBoxCollider.begin() + it);
	}

	std::vector<BoxCollider*> GetColliders() { return m_pBoxCollider; }

private:
	std::vector<BoxCollider*> m_pBoxCollider;

};
