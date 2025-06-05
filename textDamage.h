#pragma once
#include "gameObject.h"
#include "component.h"

class TextDamage : public GameObject
{
private:

	int frameCount = 0;
	int textUV;
	

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTextUV(int uv)
	{
		textUV = uv;
	}

};


