#pragma once
#include "attack.h"

class EnemySlash : public Attack
{
private:

	int frameCount = 0;
	float length = 0.0f;

	float rot = 0.0f;

	XMFLOAT3 enemyPos = {};
	XMFLOAT3 enemyRot = {};

	bool initOn = false;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void HitAttackCollision();
	void TextAttackPoint(int n);

	void SetEnemyPos(XMFLOAT3 pos) { enemyPos = pos; }
	void SetEnemyRot(XMFLOAT3 rot) { enemyRot = rot; }

	void OnInit() { initOn = true; }
};


