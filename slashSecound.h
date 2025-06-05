#pragma once
#include "attack.h"

class SlashSecound : public Attack
{
private:

	int frameCount = 0;
	float length = 0.0f;

	float rot = 0.0f;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void HitAttackCollision();
	void TextAttackPoint(int n);
};


