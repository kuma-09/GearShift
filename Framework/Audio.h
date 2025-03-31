#pragma once
#include "DeviceResources.h"
#include <Audio.h>

/// <summary>
/// 音声ファイル再生クラス
/// シングルトン
/// </summary>
class Audio
{
public:

	// BGMの種類
	enum BGMType
	{
		Title,
		Battle,
	};

	// デストラクタ
	~Audio();
	// Audioクラスのインスタンスを取得する
	static Audio* GetInstance();

	// 音声ファイルを読み込み
	void LoadSoundFile();

	// 更新処理
	void Update();

	// SE再生用関数
	void PlaySoundSE_Rocket();
	void PlaySoundSE_Hit();
	void PlaySoundSE_PowerUp();
	void PlaySoundSE_Boost();
	void PlaySoundSE_Slash();
	void PlaySoundSE_Explosion();

	// BGM再生用関数
	void PlaySoundBGM(BGMType type);

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
	std::unique_ptr<DirectX::SoundEffect> m_rocketLauncherSE;
	std::unique_ptr<DirectX::SoundEffect> m_hitSE;
	std::unique_ptr<DirectX::SoundEffect> m_powerUpSE;
	std::unique_ptr<DirectX::SoundEffect> m_boostSE;
	std::unique_ptr<DirectX::SoundEffect> m_slashSE;
	std::unique_ptr<DirectX::SoundEffect> m_explosionSE;
	std::unique_ptr<DirectX::SoundEffect> m_titleBGM;
	std::unique_ptr<DirectX::SoundEffect> m_battleBGM;
	std::unique_ptr<DirectX::SoundEffectInstance> m_soundEffectInstanceBGM;

	// ボリューム
	float m_bgmVolume = 0.05f;
	float m_seVolume  = 0.1f;
};