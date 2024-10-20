#include "pch.h"
#include "Audio.h"

std::unique_ptr<Audio> Audio::m_audio = nullptr;

Audio::~Audio()
{

}

Audio* const Audio::GetInstance()
{
	if (m_audio == nullptr)
	{
		//m_audio.reset(new Audio());
	}
	return m_audio.get();
}
