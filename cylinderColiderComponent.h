#pragma once
//#include "component.h"
#include "gameObject.h"
#include "colider.h"
#include <tuple>

class CylinderColiderComponent : public Colider
{
private:

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


	bool IsCollision();
	std::tuple<bool, GameObject*,std::list<GameObject*>> GetCollision() override;

};

