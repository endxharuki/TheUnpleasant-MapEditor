#pragma once
#include "gameObject.h"
#include "component.h"

class TitleBoard : public GameObject
{
private:

	int frameCount = 0;
	bool doOnce = false;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;



};


