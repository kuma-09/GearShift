#pragma once 
#include "Game/Player/State.h"
#include "Game/Parts/IPart.h"
#include "Framework/InputManager.h"
#include "Game/Player/Player.h"

class Idol : public State
{
public:
	Idol();
	~Idol();

	void Initialize(Player* player);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	// コンポーネントを追加
	template<typename PartType>
	std::weak_ptr<PartType> AddPart()
	{
		std::shared_ptr<PartType> newPart = std::make_shared<PartType>();
		m_umPart[typeid(PartType)] = newPart;
		newPart->SetOwner(this);
		return newPart;
	}

	// コンポーネントを取得
	template<typename PartType>
	std::weak_ptr<PartType> GetPart()
	{
		return std::static_pointer_cast<PartType>(m_umPart[typeid(PartType)]);
	}

	// コンポーネントを更新
	void PartUpdate(float elapsedTime)
	{
		for (auto&& spPart : m_umPart)
		{
			spPart.second->Update(elapsedTime);
		}
	}

	void SetTarget(GameObject* target);

private:
	Player* m_player;

	InputManager* m_inputManager;

	GameObject* m_target;

	std::unordered_map<std::type_index, std::shared_ptr<IPart>> m_umPart;
};
