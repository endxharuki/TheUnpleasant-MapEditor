#pragma once

#include <xaudio2.h>
#include "gameObject.h"
#include "GameObjectComponent.h"


class Audio : public GameObjectComponent
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice{};
	BYTE*					m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};


public:
	static void InitMaster();
	static void UninitMaster();

	using GameObjectComponent::GameObjectComponent;

	void Uninit();

	void Load(const char *FileName);
	void Play(bool Loop = false);
	void SetVolume(float volume) {

		m_MasteringVoice->SetVolume(volume);

	}

};

