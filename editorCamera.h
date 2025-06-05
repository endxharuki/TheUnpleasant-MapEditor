#pragma once
#include <cstddef>
#include "gameObject.h"
#include "component.h"

class EditorCamera : public GameObject
{
private:

	//XMFLOAT3 m_Position{};
	XMFLOAT3 m_Target{};
	XMFLOAT3 m_Up{0.0f,1.0f,0.0f};

	float m_Radian = 0.0f;
	float m_Angle = 0.0f;//É∆
	float m_Fai = 0.0f;//É≥
	float m_Len = 0.0f;

	XMMATRIX m_ViewMatrix{};
	XMMATRIX m_ProjectionMatrix{};

	bool OnCursor = false;

	POINT m_MousePoint;

	float m_CursorX, m_CursorY;
	float m_OldCursorX, m_OldCursorY;
	float cameraSpeed = 0.1f;
	XMFLOAT2 m_Sensitivity = { 0.02f,0.01f };
	
	XMFLOAT2 m_DragPos = { 0.0f,0.0f };
	XMFLOAT2 difference = { 0,0 };

	bool doOnce = false;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void DrawImGui() override;

	XMMATRIX GetViewMatrix();
	void SetCursorState(bool state) { OnCursor = state; }
	XMFLOAT3 GetMouseWorldPosition();
	XMFLOAT3 GetMouseRayDirection();

};

