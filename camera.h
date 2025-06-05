#pragma once
#include <cstddef>
#include "gameObject.h"
#include "component.h"

class Camera : public GameObject
{
private:

	//XMFLOAT3 m_Position{};
	XMFLOAT3 m_Target{};
	XMFLOAT3 m_Up{0.0f,1.0f,0.0f};
	XMFLOAT2 m_Len{};

	XMFLOAT4X4 m_ViewMatrix{};
	XMFLOAT3 PlayerOldPos = { 0.0f,0.0f,0.0f };

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMMATRIX GetViewMatrix();

};

