#pragma once
#include "ui.h"
#include "uiComponent.h"
#include "gameObject.h"

enum FadeState
{
	None,
	Out,
	In

};

class Fade : public UI
{
private:

	int frameCount = 0;
	float m_Alpha = 0.0f;

	
	FadeState m_FadeState = None;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	FadeState GetFadeState() const { return m_FadeState; }
	void SetFadeState(FadeState state) {
		m_FadeState = state;
		if (state == Out)
		{
			m_Alpha = 0.0f;
		}
		else if (state == In)
		{
			m_Alpha = 1.0f;
		}

	}

	
};


