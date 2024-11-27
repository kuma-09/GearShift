/*
	@file	IScene.h
	@brief	�V�[���̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#include "Game/Components/BoxCollider.h"

class Game;

class IScene
{

public:
	virtual ~IScene() = default;

	virtual void Initialize(Game* game) = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void RenderUI() = 0;

	virtual void Finalize() = 0;

	// �{�b�N�X�R���C�_�[��ǉ�
	void AddCollider(BoxCollider* collider)
	{
		m_pBoxCollider.emplace_back(collider);
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
		if (m_pBoxCollider.size() > it)
		{
			m_pBoxCollider.erase(m_pBoxCollider.begin() + it);
		}
	}

	// �{�b�N�X�R���C�_�[�̔z����擾
	std::vector<BoxCollider*> GetColliders() { return m_pBoxCollider; }

	void SetGame(Game* game) { m_game = game; }
	Game* GetGame() { return m_game; }

private:

	Game* m_game;
	// �{�b�N�X�R���C�_�[���܂Ƃ߂��z��
	std::vector<BoxCollider*> m_pBoxCollider;

};
