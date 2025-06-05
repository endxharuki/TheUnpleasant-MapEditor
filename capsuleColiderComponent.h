#pragma once
#include "gameObject.h"
#include "colider.h"
#include <tuple>
#include <vector>

class CapsuleColiderComponent : public Colider
{
private:

	//カプセルの構造体
	Capsule m_Capsule =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		0.0f
	};
	
	//中心から下と上の線分までの長さ
	float m_CenterPosLngth = 0;
	float m_TopFai = 0.0f;
	float m_BottomFai = 0.0f;
	float m_TopAngle = 0.0f;
	float m_BottomAngle = 0.0f;


	XMFLOAT3 clossPoint1 = {}, clossPoint2 = {};

	XMFLOAT3 m_Correction = {};
	bool TopBottomRegister = false;

	float m_length = 0.0f;

	bool onImGui = false;
	bool DrawColider = false;

	bool update = true;

public:

	CapsuleColiderComponent() = default;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void DrawImGui() override;
	void OnImGui()
	{
		onImGui = true;
	}
	void OnDraw()
	{
		DrawColider = true;
	}
	void CapsuleDrawState(bool draw) { DrawColider = draw; }

	void NoUpdate()
	{
		update = false;
	}

	void SetCapsuleStartPos(XMFLOAT3 pos)
	{
		m_Capsule.startPos = pos;
	}
	void SetCapsuleEndPos(XMFLOAT3 pos)
	{
		m_Capsule.endPos = pos;
	}


	void SetCenterPosLnegth(float length)
	{
		m_CenterPosLngth = length;
	}

	Capsule GetCapsule() const
	{
		return m_Capsule;
	}
	float GetCenterPosLength() const
	{
		return m_CenterPosLngth;
	}
	XMFLOAT3 GetCorrection()const
	{
		return m_Correction;
	}

	void MoveCollision() override;


	std::tuple<bool, GameObject*,std::list<GameObject*>> GetCollision() override ;
	
	

	//テスト用
	// ベクトルをボックスのローカル座標に変換
	XMFLOAT3 rotateVector(const XMFLOAT3& vec, const XMFLOAT3& axisX, const XMFLOAT3& axisY, const XMFLOAT3& axisZ) {
		return {
			vec.x * axisX.x + vec.y * axisY.x + vec.z * axisZ.x,
			vec.x * axisX.y + vec.y * axisY.y + vec.z * axisZ.y,
			vec.x * axisX.z + vec.y * axisY.z + vec.z * axisZ.z
		};
	}
};


