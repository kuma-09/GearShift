#pragma once
#ifndef AUDIO_DEFINED
#define AUDIO_DEFINED
#include "DeviceResources.h"
#include <Audio.h>

// Audioクラスを定義する
class Audio
{
public:
	// デストラクタ
	~Audio();
	// Audioクラスのインスタンスを取得する
	static Audio* GetInstance();

private:
	// コンストラクタ
	Audio();
	// インスタンスをコピーすることを禁止する
	void operator=(const Audio&) = delete;
	// インスタンスをムーブすることを禁止する
	Audio& operator= (Audio&&) = delete;
	// コピーコンストラクタは禁止する
	Audio(const Audio&) = delete;
	// ムーブコンストラクタは禁止する
	Audio(Audio&&) = delete;

private:
	// オーディオ
	static std::unique_ptr<Audio> m_audio;

	// オーディオ関連
	std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unique_ptr<DirectX::SoundEffect> m_soundEffectSE;
	std::unique_ptr<DirectX::SoundEffect> m_soundEffectBGM;
	std::unique_ptr<DirectX::SoundEffectInstance> m_soundEffectInstanceBGM;
};

#endif