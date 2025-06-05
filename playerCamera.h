#pragma once
#include <cstddef>
#include "gameObject.h"
#include "component.h"
#include <Windows.h>

constexpr auto RADIAN = (0.05f);

class PlayerCamera : public GameObject
{
private:

	//XMFLOAT3 m_Position{};
	XMFLOAT3 m_Target{};
	XMFLOAT3 m_Up{0.0f,1.0f,0.0f};
	

	XMFLOAT4X4 m_ViewMatrix{};
	XMFLOAT3 PlayerOldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 AddTargetPosition = { 0.0f,0.0f,0.0f };

	float m_Radian = 0.0f;
	float m_Angle = 0.0f;//θ
	float m_Fai = 0.0f;//Φ
	float m_Len = 0.0f;
	
	float frameCount[4] = { 0.0f,0.0f,0.0f,0.0f };

	POINT m_MousePoint;

	float m_CursorX = 0.0f, m_CursorY = 0.0f;
	float m_OldCursorX = 0.0f, m_OldCursorY = 0.0f;

	XMFLOAT2 m_Sensitivity = {0.02f,0.01f};

	//デバッグ用
	bool OnCursor = true;
	bool doOnce = false;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void CameraControl();
	void AssistCamera();

	XMMATRIX GetViewMatrix() const;

	void DrawImGui() override;

	bool CehckView(XMFLOAT3);


};

