#pragma once 
#include "Game/GameObject.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/State.h"
#include "Framework/DebugString.h"
#include "Game/Player/BoostGage.h"

#include <map>

class Idol;
class Jump;
class Boost;
class Attack;
class Bullet;
class NormalBullet;
class HomingBullet;

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
	Attack* GetAttack() { return m_attack.get(); }

	void SetPrePosition(DirectX::SimpleMath::Vector3 pos) {	m_prePosition = pos;}
	DirectX::SimpleMath::Vector3 GetPrePosition() { return m_prePosition; }
	void SetTarget(GameObject* target);
	GameObject* GetTarget() { return m_target; }
	void SetOnFloor(bool n) { m_onFloor = n; }
	bool GetOnFloor() { return m_onFloor; }
	void ChangeState(State* state);
	void Shot();

	// パーツをセット
	void SetPart(const Part::TypeID& partType, std::unique_ptr<Part> part)
	{
		m_pPart[partType] = std::move(part);
		m_pPart[partType]->SetOwner(this);
		m_pPart[partType]->Initialize(10, GetScene());
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

	// パーツをまとめて描画
	void RenderParts() {
		for (auto& pair : m_pPart) 
		{
			pair.second->Render(GetWorld());
		}
	}

	void Collision(BoxCollider* collider);

private:
	void Reload();


private:

	InputManager* m_inputManager;
	
	// ターゲット
	GameObject* m_target;

	// パーツ配列
	std::unordered_map<Part::TypeID, std::unique_ptr<Part>> m_pPart;

	// 弾の最大数
	static const int MAX_BULLET_COUNT = 50;

	// 一フレーム前の座標
	DirectX::SimpleMath::Vector3 m_prePosition;

	// ステート
	State* m_state;
	std::unique_ptr<Idol> m_idol;
	std::unique_ptr<Jump> m_jump;
	std::unique_ptr<Boost> m_boost;
	std::unique_ptr<Attack> m_attack;

	// 弾配列
	std::unique_ptr<Bullet> m_bullet[MAX_BULLET_COUNT];

	std::unique_ptr<DebugString> m_debugString;

	bool m_onFloor;

};
