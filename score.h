#pragma once
#include "ui.h"
#include "uiComponent.h"


class Score : public UI
{
private:

	int m_Score = 0;

	float m_AnimetionX = 5.0f;
	float m_AnimetionY = 5.0f;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;



};


