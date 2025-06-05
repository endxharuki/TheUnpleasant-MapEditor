#pragma once
#include "gameObject.h"
#include "component.h"
#include "particleSpowner.h"

class Star : public GameObject
{
private:

	ParticleSpowner* m_Particle = nullptr;

	int frameCount = 0;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;



};


