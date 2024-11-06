#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Particle/Emitter.h"
#include "Player/Player.h"
#include "Enemy/FixedEnemy.h"
#include "Enemy/HomingEnemy.h"
#include "Game/Object/Wall/Wall.h"
#include "Game/Object/DropItem.h"
#include "Game/Object/DropItemB.h"
#include "Game/Object/SkyDome.h"
#include "Game/Object/Floor.h"

#include "Game/Player/TargetArea.h"
#include "Shader/PostProcess.h"

#include "Framework/DebugString.h"

class Camera;
class HPUI;
class HitParticle;
class HitEffect;
class StartAnimation;
class BulletMagazine;
class ExBulletMagazine;


class PlayScene final: public IScene
{
public:
	PlayScene();
	~PlayScene();

	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;

	void UpdateBulletMagazine();

	// �q�b�g�G�t�F�N�g�𐶐�����֐�
	void SetNoise();
	void CreateHitParticle(DirectX::SimpleMath::Matrix world);
	void CreateHitParticle(DirectX::SimpleMath::Matrix world,DirectX::SimpleMath::Quaternion rotate);
	void CreateHitEffect(DirectX::SimpleMath::Vector3 pos);
private:

	void CreateShadow();

	// �G���폜
	std::vector<std::unique_ptr<Enemy>>::iterator RemoveEnemy(std::vector<std::unique_ptr<Enemy>>::iterator it);

	// �A�C�e�����폜
	void RemoveItem(std::vector<std::unique_ptr<DropItem>>::iterator it);
	void RemoveItemB(std::vector<std::unique_ptr<DropItemB>>::iterator it);

private:

	
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	// �v���C���[
	std::unique_ptr<Player> m_player;
	// �G�l�~�[�z��
	std::vector<std::unique_ptr<Enemy>> m_Enemy;
	// �ǔz��
	std::vector<std::unique_ptr<Wall>> m_wall;
	// �A�C�e���z��
	std::vector<std::unique_ptr<DropItem>> m_dropItem;
	std::vector<std::unique_ptr<DropItemB>> m_dropItemB;
	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> m_skyDome;
	// ��
	std::vector<std::unique_ptr<Floor>>   m_floor;
	// �^�[�Q�b�g�͈�
	std::unique_ptr<TargetArea> m_targetArea;
	// HP�̕\��UI
	std::unique_ptr<HPUI> m_hpUI;
	// �q�b�g�p�[�e�B�N��
	std::vector<std::unique_ptr<HitParticle>> m_hitParticle;
	// ���j�G�t�F�N�g
	std::unique_ptr<HitEffect> m_hitEffect;


	std::unique_ptr<StartAnimation> m_startAnimation;
	std::unique_ptr<BulletMagazine> m_bulletMagazine;
	std::unique_ptr<ExBulletMagazine> m_exBulletMagazine;

	// �o�ߎ���
	float m_totalTime;
	// �c�莞��
	float m_timeLimit;
	
	std::unique_ptr<PostProcess> m_postProcess;

};