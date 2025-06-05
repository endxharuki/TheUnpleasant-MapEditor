#pragma once
#include "gameObject.h"
#include "gameObjectComponent.h"
#include "texture2DComponent.h"
#include <tuple>
#include "textrue2D.h"
#include "vector"

class Circle2DColider : public GameObjectComponent
{

private:



	XMFLOAT2 m_CenterPosition = { 0.0f,0.0f };
	float m_Radius = 0.0f;

	XMFLOAT3 m_TargetWorldPosition = { 0.0f,0.0f,0.0f };
	XMFLOAT2 m_TargetPosition = { 0.0f,0.0f };

	XMMATRIX m_ViewMatrix = {};
	XMMATRIX m_ProjectionMatrix = {};

	bool m_Update = true;
	bool m_Draw = true;

	Texture2D* m_Texture;
	std::vector<Texture2D*> m_MagicUIList;
	int m_OldUICount = 0;

	bool onImGui = false;

	bool InObject = false;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPosition(XMFLOAT2 pos) { m_CenterPosition = pos; }
	void SetRadius(float r) { m_Radius = r; }

	void SetViewMatrix(XMMATRIX matrix) { m_ViewMatrix = matrix; }
	void SetProjectionMatrix(XMMATRIX matrix) { m_ProjectionMatrix = matrix; }

	XMFLOAT2 GetPosition() const { return m_CenterPosition; }
	float Getradius() const { return m_Radius; }

	bool ObjectInCircle(XMFLOAT3 worldPos,int screenWidth, int screenHeight,float &screenPosX,float &screenPosY);
	void OnImGui()
	{
		onImGui = true;
	}

	void DrawImGui();

	void CircleUpdateState(bool state) { m_Update = state; }
	void CircleDrawState(bool state) { m_Draw = state; }

	XMFLOAT2 GetTargetPosition() const { return m_TargetPosition; }
	bool GetIsInObject() const { return InObject; }

	XMFLOAT3 GetTragetWorldPosition() const{ return m_TargetWorldPosition; }
};

