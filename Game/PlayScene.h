#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Particle/Emitter.h"
#include "Game/Object/DropItem.h"
#include "Game/Object/DropItemB.h"
#include "Game/Object/SkyDome.h"
#include "Game/Object/Floor.h"
#include "Game/Object/Floor/Floor2.h"
#include "Game/Player/Player.h"
#include "Game/Player/TargetArea.h" 
#include "Shader/PostProcess.h"
#include "Framework/DebugString.h"
#include "UI/Number.h"
#include "UI/Menu.h"

#include "Game/System/Tutorial.h"

class Camera;
class Reload;
class HitParticle;
class ExplosionEffect;
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
	void TranslucentRender() override;
	void RenderUI() override;
	void Finalize() override;

	void CreateHitParticle(DirectX::SimpleMath::Vector3 pos,float size = 1.0f);
	void CreateHitEffect(DirectX::SimpleMath::Vector3 pos);
private:
	void CreateObject(std::string className, DirectX::SimpleMath::Vector3 pos);
	void CreateMenu();
	void UpdateTargetArea();
	void UpdateParticle(float elapsedTime);
	void UpdateMenu();
private:

	
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;


	// �Q�[���I�u�W�F�N�g���܂Ƃ߂��|�C���^�z��
	std::vector<GameObject*> m_gameObjects;
	// �v���C���[
	std::weak_ptr<GameObject> m_player;
	// �A�C�e���z��
	std::vector<std::unique_ptr<DropItem>> m_dropItem;
	std::vector<std::unique_ptr<DropItemB>> m_dropItemB;
	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> m_skyDome;
	// ��
	std::unique_ptr<Floor>   m_floor;
	// �^�[�Q�b�g�͈�
	std::unique_ptr<TargetArea> m_targetArea;

	// �q�b�g�p�[�e�B�N��
	std::vector<std::unique_ptr<HitParticle>> m_hitParticle;
	// ���j�G�t�F�N�g
	std::unique_ptr<ExplosionEffect> m_hitEffect;
	
	std::unique_ptr<StartAnimation> m_startAnimation;

	// �|�[�Y���̃��j���[
	std::unique_ptr<Menu> m_menu;
	// ���j���[�̔w�i
	std::unique_ptr<UI>   m_menuBack;
	// �`���[�g���A��
	std::unique_ptr<Tutorial> m_tutorial;

	// �o�ߎ��ԕ`��N���X
	std::unique_ptr<Number> m_time;

	// �o�ߎ���
	float m_totalTime;
	// �c�莞��
	float m_timeLimit;
	// ���j���[���J���Ă邩
	bool  m_isMenu;
};