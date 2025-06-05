#pragma once
#include "gameObject.h"
#include "component.h"

class TitleHideBoard : public GameObject
{
private:

	int frameCount = 0;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;



};


