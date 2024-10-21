#pragma once
#ifndef AUDIO_DEFINED
#define AUDIO_DEFINED
#include "DeviceResources.h"
#include <Audio.h>

// Audio�N���X���`����
class Audio
{
public:
	// �f�X�g���N�^
	~Audio();
	// Audio�N���X�̃C���X�^���X���擾����
	static Audio* GetInstance();

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
	std::unique_ptr<DirectX::SoundEffect> m_soundEffectSE;
	std::unique_ptr<DirectX::SoundEffect> m_soundEffectBGM;
	std::unique_ptr<DirectX::SoundEffectInstance> m_soundEffectInstanceBGM;
};

#endif