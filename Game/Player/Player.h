#pragma once 
#include "Game/GameObject.h"
#include "Game/Parts/IPart.h"
#include "Framework/InputManager.h"
#include "Game/Player/State/State.h"
#include "Framework/DebugString.h"

#include <map>

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

	Idol* GetIdol() { return m_idol.get(); }
	Jump* GetJump() { return m_jump.get(); }
	Boost* GetBoost() { return m_boost.get(); }

	void SetTarget(GameObject* target);

	void ChangeState(State* state);

	// パーツのセッターゲッター
	void SetPart(const std::string& partType, std::unique_ptr<IPart> part)
	{
		m_pPart[partType] = std::move(part);
		m_pPart[partType]->SetOwner(this);
		m_pPart[partType]->Initialize(10, GetScene());
	}

	IPart* GetPart(const std::string& partType)
	{
		if (m_pPart.find(partType) != m_pPart.end()) 
		{
			return m_pPart[partType].get();
		}
		return nullptr;
	}

	void UpdateParts(float elapsedTime) {
		for (auto& pair : m_pPart) 
		{
			pair.second->Update(elapsedTime);
		}
	}

	void RenderParts() {
		for (auto& pair : m_pPart) 
		{
			pair.second->Render(GetWorld());
		}
	}

private:

	InputManager* m_inputManager;
	
	GameObject* m_target;

	std::unordered_map<std::string, std::unique_ptr<IPart>> m_pPart;

	State* m_state;
	std::unique_ptr<Idol> m_idol;
	std::unique_ptr<Jump> m_jump;
	std::unique_ptr<Boost> m_boost;
	std::unique_ptr<Bullet> m_bullet;

	std::unique_ptr<DebugString> m_debugString;


};
