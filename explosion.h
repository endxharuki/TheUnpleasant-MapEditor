#pragma once
#include "effect.h"

class Explosion : public Effect
{
private:


public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};


