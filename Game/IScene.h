/*
	@file	IScene.h
	@brief	シーンのインターフェイスクラス
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

	virtual void Finalize() = 0;



	void SetGame(Game* game) { m_game = game; }
	Game* GetGame() { return m_game; }

private:

	Game* m_game;

};
