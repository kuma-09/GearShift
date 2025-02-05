#include "pch.h"
#include "Audio.h"

std::unique_ptr<Audio> Audio::m_audio = nullptr;

Audio::Audio()
{

}

Audio::~Audio()
{
	// オーディオエンジンの後始末
	m_soundEffectInstanceBGM->Stop(true);
	if (m_audioEngine)
	{
		m_audioEngine->Suspend();
	}
}

Audio* Audio::GetInstance()
{
	if (m_audio == nullptr)
	{
		m_audio.reset(new Audio());
	}
	return m_audio.get();
}

void Audio::LoadSoundFile()
{
	using namespace DirectX;

	// オーディオエンジンのフラグ
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif

	// オーディオエンジンを作成する
	m_audioEngine = std::make_unique<AudioEngine>(eflags);

	// サウンドエフェクトSEをロードする
	m_rocketLauncherSE = std::make_unique<SoundEffect>(
		m_audioEngine.get(), L"Resources/Sounds/RocketLauncher.wav");

	m_hitSE = std::make_unique<SoundEffect>(
		m_audioEngine.get(), L"Resources/Sounds/Hit.wav");

	m_powerUpSE = std::make_unique<SoundEffect>(
		m_audioEngine.get(), L"Resources/Sounds/PowerUp.wav");

	m_boostSE = std::make_unique<SoundEffect>(
		m_audioEngine.get(), L"Resources/Sounds/Boost.wav");

	m_slashSE = std::make_unique<SoundEffect>(
		m_audioEngine.get(), L"Resources/Sounds/Slash2.wav");

	m_explosionSE = std::make_unique<SoundEffect>(
		m_audioEngine.get(), L"Resources/Sounds/Explosion.wav");

	// サウンドエフェクトBGMをロードする
	m_titleBGM = std::make_unique<SoundEffect>(
		m_audioEngine.get(), L"Resources/Sounds/Observer.wav");

	m_battleBGM = std::make_unique<SoundEffect>(
		m_audioEngine.get(), L"Resources/Sounds/Fractured_Bonds.wav");

	// サウンドエフェクトBGMをインスタンス化する
	m_soundEffectInstanceBGM = m_titleBGM->CreateInstance();

	m_soundEffectInstanceBGM->SetVolume(m_bgmVolume);
	m_soundEffectInstanceBGM->Play(true);

}

void Audio::Update()
{
	// オーディオエンジンを更新する
	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			OutputDebugString(L"AudioEngine Error!\n");
		}
	}
}

void Audio::PlaySoundSE_Rocket()
{
	m_rocketLauncherSE->Play(m_seVolume, 0.5f, 0.5f);
}

void Audio::PlaySoundSE_Hit()
{
	m_hitSE->Play(m_seVolume, 0.5f, 0.5f);
}

void Audio::PlaySoundSE_PowerUp()
{
	m_powerUpSE->Play(m_seVolume, 0.5f, 0.5f);
}

void Audio::PlaySoundSE_Boost()
{
	m_boostSE->Play(m_seVolume, 0.5f, 0.5f);
}

void Audio::PlaySoundSE_Slash()
{
	m_slashSE->Play(m_seVolume, 0.5f, 0.5f);
}

void Audio::PlaySoundSE_Explosion()
{
	m_explosionSE->Play(m_seVolume, 0.5f, 0.5f);
}

void Audio::ChageBGM(BGMType type)
{
	switch (type)
	{
	case Audio::Title:
		m_soundEffectInstanceBGM = m_titleBGM->CreateInstance();
		m_soundEffectInstanceBGM->SetVolume(m_bgmVolume);
		m_soundEffectInstanceBGM->Play(true);
		break;
	case Audio::Battle:
		m_soundEffectInstanceBGM = m_battleBGM->CreateInstance();
		m_soundEffectInstanceBGM->SetVolume(m_bgmVolume);
		m_soundEffectInstanceBGM->Play(true);
		break;
	default:
		break;
	}
}
