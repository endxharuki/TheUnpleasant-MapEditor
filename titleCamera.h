#pragma once
#include <cstddef>
#include "gameObject.h"
#include "component.h"

class TitleCamera : public GameObject
{
private:

	//XMFLOAT3 m_Position{};
	XMFLOAT3 m_Target{};
	XMFLOAT3 m_Up{0.0f,1.0f,0.0f};

	float m_Radian = 0.0f;
	float m_Angle = 0.0f;//É∆
	float m_Fai = 0.0f;//É≥
	float m_Len = 0.0f;

	XMFLOAT4X4 m_ViewMatrix{};
	XMFLOAT3 PlayerOldPos = { 0.0f,0.0f,0.0f };

	bool OnCursor = true;

	POINT m_MousePoint;

	float m_CursorX, m_CursorY;
	float m_OldCursorX, m_OldCursorY;

	XMFLOAT2 m_Sensitivity = { 0.02f,0.01f };

	XMFLOAT2 difference = { 0,0 };

	float m_UpDownFrame = 0.0f;
	float m_StopFrame = 0.0f;
	bool m_ChangeUpDown = false;
	bool m_Stop = false;

	float UpDownSpeed = 0.0f;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	XMMATRIX GetViewMatrix();

	bool GetMoveState() const { return m_Stop; }

};

