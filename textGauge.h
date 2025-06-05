#pragma once
#include "ui.h"
#include "uiComponent.h"


class TextGauge : public UI
{
private:

	int m_GaugePoint = 0;
	float firstMp = 0.0f;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;



};


