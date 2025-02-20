#pragma once
#ifndef AUDIO_DEFINED
#define AUDIO_DEFINED
#include "DeviceResources.h"
#include <Audio.h>

// Audio�N���X���`����
class Audio
{
public:

	enum BGMType
	{
		Title,
		Battle,
	};

	// �f�X�g���N�^
	~Audio();
	// Audio�N���X�̃C���X�^���X���擾����
	static Audio* GetInstance();

	// �����t�@�C����ǂݍ���
	void LoadSoundFile();

	void Update();

	void PlaySoundSE_Rocket();
	void PlaySoundSE_Hit();
	void PlaySoundSE_PowerUp();
	void PlaySoundSE_Boost();
	void PlaySoundSE_Slash();
	void PlaySoundSE_Explosion();

	void ChageBGM(BGMType type);

private:
	// �R���X�g���N�^
	Audio();
	// �C���X�^���X���R�s�[���邱�Ƃ��֎~����
	void operator=(const Audio&) = delete;
	// �C���X�^���X�����[�u���邱�Ƃ��֎~����
	Audio& operator= (Audio&&) = delete;
	// �R�s�[�R���X�g���N�^�͋֎~����
	Audio(const Audio&) = delete;
	// ���[�u�R���X�g���N�^�͋֎~����
	Audio(Audio&&) = delete;

private:
	// �I�[�f�B�I
	static std::unique_ptr<Audio> m_audio;

	// �I�[�f�B�I�֘A
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

	float m_bgmVolume = 0.0f;
	float m_seVolume  = 0.0f;
};

#endif