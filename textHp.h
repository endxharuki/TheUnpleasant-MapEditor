#pragma once
#include "ui.h"
#include "uiComponent.h"
#include "player.h"

class TextHp : public UI
{
private:

	Player* m_Player = nullptr;

	int m_GaugePoint = 0;
	float firstHp = 0.0f;


public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};


