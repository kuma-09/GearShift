#pragma once 
#include "Game/GameObject.h"
#include "Game/Parts/Part.h"
#include "Framework/InputManager.h"
#include "Game/State.h"
#include "Framework/DebugString.h"
#include "Game/Player/BoostGage.h"
#include "Game/UI/ReloadUI.h"
#include "Game/UI/BulletMagazine.h"
#include "Game/UI/ExBulletMagazine.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Player/Gun.h"
#include "Game/Player/MissileLauncher.h"
#include "Game/Object/Sword.h"
#include "Game/Object/Burner/Burner.h"
#include "Game/Object/Camera/Camera.h"
#include <map>

class Idol;
class Jump;
class Boost;
class Attack;
class Bullet;
class NormalBullet;
class HomingBullet;
class EnergyGage;

class Player : public GameObject
{
public:
	Player(IScene* scene);
	~Player();

	void Initialize();
	void Update(float elapsedTime);
	void RenderPlayerUI();
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
	float GetBoostPoint();
	GameObject* GetCamera() { return m_camera.get(); }
	EnergyGage* GetEnergyGage() { return m_energyGage.get(); }

	void SetTrailPosition(DirectX::SimpleMath::Vector3 pos);
	void ClearTrail();
	void ChangeState(State* state);
	void Shot();
	void SwordAttack();

	void Collision(Collider* collider);

private:
	void CreateState();
	void CreateDefaultParts();
	void CreateBullets();

private:

	InputManager* m_inputManager;
	
	// ターゲット
	GameObject* m_target;

	// パーツ配列
	std::unordered_map<Part::TypeID, std::unique_ptr<Part>> m_pPart;

	// 弾の最大数
	static const int MAX_BULLET_COUNT = 30;
	// 追加弾の最大数
	static const int MAX_EXBULLET_COUNT = 10;

	// 一フレーム前の座標
	DirectX::SimpleMath::Vector3 m_prePosition;

	// ステート
	State* m_state;
	std::unique_ptr<Idol> m_idol;
	std::unique_ptr<Jump> m_jump;
	std::unique_ptr<Boost> m_boost;
	std::unique_ptr<Attack> m_attack;

	// 案オーダー度マップを使ったステート
	//std::unordered_map<type,std::unordered_map<stateType,State>> m_stateMap;

	std::unique_ptr<Camera> m_camera;

	// 武器
	std::unique_ptr<Gun>			 m_gun;
	std::unique_ptr<MissileLauncher> m_missileLauncher;

	std::unique_ptr<Burner> m_burner;

	std::unique_ptr<EnergyGage> m_energyGage;
	std::unique_ptr<ReloadUI> m_reload;

	std::unique_ptr<BulletMagazine>   m_bulletMagazine;
	std::unique_ptr<ExBulletMagazine> m_exBulletMagazine;

	DirectX::SimpleMath::Vector3 m_trailPosition;
	bool m_onFloor;

	int m_bulletType;

};
