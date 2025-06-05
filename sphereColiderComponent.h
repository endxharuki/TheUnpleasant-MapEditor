#pragma once
#include "gameObject.h"
#include "colider.h"
#include <tuple>

class SphereColiderComponent : public Colider
{
private:

	Sphere m_Sphere =
	{
		{0.0f,0.0f,0.0f},
		0.0f,
	};

public:

	SphereColiderComponent() = default;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool SphereOBBCollision(const Box box, Sphere sphere, XMFLOAT3& collisionpoint);
	void ReflectSphere(Sphere sphere, const XMFLOAT3 normal);

	std::tuple<bool, GameObject*,std::list<GameObject*>> GetCollision()  override;
	
	Sphere GetSphere() const
	{
		return m_Sphere;
	}
};


