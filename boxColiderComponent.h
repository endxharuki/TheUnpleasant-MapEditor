#pragma once
#include "gameObject.h"
#include "colider.h"
#include <tuple>
#include <cmath>

class BoxColiderComponent : public Colider
{
private:


	Box m_OBB =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}}
	};


public:

	BoxColiderComponent() = default;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void MoveCollision() override;
	Box GetOBB() const
	{
		return m_OBB;
	}
	

	std::tuple<bool, GameObject*,std::list<GameObject*>> GetCollision()  override ;

	
	

};


