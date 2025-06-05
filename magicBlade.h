#pragma once
#include "attack.h"

class MagicBlade : public Attack
{
private:

	int frameCount = 0;
	float length = 0.0f;

	float rot = 0.0f;

	bool m_OnBurst = false;
	XMFLOAT3 m_TargetVector = { 0.0f,0.0f,0.0f };


public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void HitAttackCollision();
	void TextAttackPoint(int n);

	void SetBurstState(bool state) { m_OnBurst = state; }
	void SetTargetVector(XMFLOAT3 vec) { m_TargetVector = vec; }

};


