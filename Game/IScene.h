/*
	@file	IScene.h
	@brief	�V�[���̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#include "Game/Components/Collider.h"

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
	void AddCollider(Collider* collider)
	{
		m_pCollider.emplace_back(collider);
	}

	// �R���C�_�[���폜����
	void RemoveCollider(Collider* collider)
	{
		int it = 0;
		for (auto Colliders : m_pCollider)
		{
			if (Colliders == collider)
			{
				break;
			}
			it++;
		}
		if (m_pCollider.size() > it)
		{
			m_pCollider.erase(m_pCollider.begin() + it);
		}
	}

	// �{�b�N�X�R���C�_�[�̔z����擾
	std::vector<Collider*> GetColliders() { return m_pCollider; }

	void SetGame(Game* game) { m_game = game; }
	Game* GetGame() { return m_game; }

private:

	Game* m_game;
	// �{�b�N�X�R���C�_�[���܂Ƃ߂��z��
	std::vector<Collider*> m_pCollider;

};
