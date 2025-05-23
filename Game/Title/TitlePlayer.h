#pragma once 
#include "Game/GameObject.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/IState.h"
#include "Framework/DebugString.h"
#include "Game/Player/BoostGage.h"

#include "Game/Player/Gun.h"

#include <map>

class TitlePlayer : public GameObject
{
public:
	TitlePlayer(Scene* scene);
	~TitlePlayer();

	void Initialize();
	void Update(float elapsedTime);
	void Finalize();

	// パーツをセット
	void SetPart(const Part::TypeID& partType, std::unique_ptr<Part> part)
	{
		m_pPart[partType] = std::move(part);
		m_pPart[partType]->SetOwner(this);
		m_pPart[partType]->Initialize();
	}

	// パーツを取得
	Part* GetPart(const Part::TypeID& partType)
	{
		if (m_pPart.find(partType) != m_pPart.end())
		{
			return m_pPart[partType].get();
		}
		return nullptr;
	}

	// パーツをまとめて更新
	void UpdateParts(float elapsedTime) {
		for (auto& pair : m_pPart)
		{
			pair.second->Update(elapsedTime);
		}
	}

	void Collision(Collider* collider);

private:

	// パーツ配列
	std::unordered_map<Part::TypeID, std::unique_ptr<Part>> m_pPart;
	std::unique_ptr<Gun> m_gun;

};
