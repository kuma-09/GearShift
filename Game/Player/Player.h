#pragma once 
#include "Game/GameObject.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/State.h"
#include "Framework/DebugString.h"
#include "Game/Player/BoostGage.h"
#include "Game/Object/Bullet/Bullet.h"

#include <map>

class Idol;
class Jump;
class Boost;
class Attack;
class Bullet;
class NormalBullet;
class HomingBullet;
class BoostGage;

class Player : public GameObject
{
public:
	Player(IScene* scene);
	~Player();

	void Initialize();
	void Update(float elapsedTime);
	void CreateShadow();
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

	float GetBoostPoint();

	// �p�[�c���Z�b�g
	void SetPart(const Part::TypeID& partType, std::unique_ptr<Part> part)
	{
		m_pPart[partType] = std::move(part);
		m_pPart[partType]->SetOwner(this);
		m_pPart[partType]->Initialize(10, GetScene());
	}

	// �p�[�c���擾
	Part* GetPart(const Part::TypeID& partType)
	{
		if (m_pPart.find(partType) != m_pPart.end()) 
		{
			return m_pPart[partType].get();
		}
		return nullptr;
	}

	// �p�[�c���܂Ƃ߂čX�V
	void UpdateParts(float elapsedTime) {
		for (auto& pair : m_pPart) 
		{
			pair.second->Update(elapsedTime);
		}
	}

	// �p�[�c���܂Ƃ߂ĕ`��
	void RenderParts() {
		for (auto& pair : m_pPart) 
		{
			pair.second->Render();
		}
	}

	void CreateShadows()
	{
		for (auto& pair : m_pPart)
		{
			pair.second->CreateShadow();
		}
	}

	void Collision(BoxCollider* collider);
	
	void AddWepon(std::vector<std::unique_ptr<Bullet>> bullets)
	{

		m_exBullet = std::move(bullets);

		for (auto& bullet: m_exBullet)
		{
			bullet->Initalize(this);
		}
	}

	void UseBoostGage() { m_boostGage->UseBoostPoint(1); }

private:
	void Reload();


private:

	InputManager* m_inputManager;
	
	// �^�[�Q�b�g
	GameObject* m_target;

	// �p�[�c�z��
	std::unordered_map<Part::TypeID, std::unique_ptr<Part>> m_pPart;

	// �e�̍ő吔
	static const int MAX_BULLET_COUNT = 10;

	// ��t���[���O�̍��W
	DirectX::SimpleMath::Vector3 m_prePosition;

	// �X�e�[�g
	State* m_state;
	std::unique_ptr<Idol> m_idol;
	std::unique_ptr<Jump> m_jump;
	std::unique_ptr<Boost> m_boost;
	std::unique_ptr<Attack> m_attack;

	std::unique_ptr<BoostGage> m_boostGage;

	// �e�z��
	std::vector<std::unique_ptr<Bullet>> m_defaultBullet;
	// �ǉ��̒e�z��
	std::vector<std::unique_ptr<Bullet>> m_exBullet;

	bool m_onFloor;

};
