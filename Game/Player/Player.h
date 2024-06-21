#pragma once 
#include "Game/GameObject.h"
#include "Game/Parts/IPart.h"
#include "Framework/InputManager.h"
#include "Game/Player/State/State.h"

class Idol;
class Jump;
class Boost;
class Bullet;

class Player : public GameObject
{
public:
	Player(IScene* scene);
	~Player();

	void Initialize();
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

	Idol* GetIdol() { return m_idol.get(); }
	Jump* GetJump() { return m_jump.get(); }
	Boost* GetBoost() { return m_boost.get(); }

	void SetTarget(GameObject* target);

	void ChangeState(State* state);

private:

	InputManager* m_inputManager;
	
	GameObject* m_target;

	std::unordered_map<std::type_index, std::shared_ptr<IPart>> m_umPart;

	State* m_state;
	std::unique_ptr<Idol> m_idol;
	std::unique_ptr<Jump> m_jump;
	std::unique_ptr<Boost> m_boost;
	std::unique_ptr<Bullet> m_bullet;


};
