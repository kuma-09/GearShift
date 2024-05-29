/*
	@file	IScene.h
	@brief	シーンのインターフェイスクラス
*/
#pragma once

class IScene
{

public:
	virtual ~IScene() = default;

	virtual void Initialize() = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;



};
