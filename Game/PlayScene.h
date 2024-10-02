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
#include "Game/Object/SkyDome.h"
#include "Game/Object/Floor.h"

#include "Game/Player/TargetArea.h"

#include "Framework/DebugString.h"

class Camera;
class HPUI;
class HitParticle;


class PlayScene final: public IScene
{
public:
	PlayScene();
	~PlayScene();

	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;

	// �{�b�N�X�R���C�_�[��ǉ�
	void AddCollider(BoxCollider* collider)
	{
		m_pBoxCollider.push_back(collider);
	}

	// �R���C�_�[���폜����
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
		m_pBoxCollider.erase(m_pBoxCollider.begin() + it);
	}

	// �{�b�N�X�R���C�_�[�̔z����擾
	std::vector<BoxCollider*> GetColliders() { return m_pBoxCollider; }

	// �q�b�g�G�t�F�N�g�𐶐�����֐�
	void CreateHitParticle(DirectX::SimpleMath::Matrix world);

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
	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> m_skyDome;
	// ��
	std::vector<std::unique_ptr<Floor>>   m_floor;

	// �^�[�Q�b�g�͈�
	std::unique_ptr<TargetArea> m_targetArea;

	// HP�̕\��UI
	std::unique_ptr<HPUI> m_hpUI;


	// �{�b�N�X�R���C�_�[���܂Ƃ߂��z��
	std::vector<BoxCollider*> m_pBoxCollider;

	// �f�o�b�O����
	std::unique_ptr<DebugString> m_debugString;

	// �q�b�g�G�t�F�N�g
	std::vector<std::unique_ptr<HitParticle>> m_hitParticle;

	// �o�ߎ���
	float m_totalTime;
	// �c�莞��
	float m_timeLimit;
	
};