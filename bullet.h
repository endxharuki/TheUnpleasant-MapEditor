#pragma once
#include "gameObject.h"
#include "component.h"

class Bullet : public GameObject
{
private:

	float frameCount = 0.0f;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;



};

