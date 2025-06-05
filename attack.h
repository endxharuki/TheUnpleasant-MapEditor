#pragma once
#include "gameObject.h"
#include "component.h"

class Player;

class Attack : public GameObject
{

private:


protected:

	std::list<GameObject*> m_HitObjectList;

	float m_HitCount = 0.0f;
	int m_AttackPoint = 0;


public:

	virtual void Init()override{}
	virtual void Uninit()override {}
	virtual void Update()override {}
	virtual void Draw()override {}


};

