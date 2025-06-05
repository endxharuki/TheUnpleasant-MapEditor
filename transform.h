#pragma once
#include "gameObjectComponent.h"
#include "renderer.h"
#include "modelRenderer.h"
#include <cstddef>


class Transform : public GameObjectComponent
{
private:


protected:

	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };

	//動く時のベクトル
	XMFLOAT3 m_Velocity = { 0.0f,0.0f,0.f };


	XMFLOAT3 m_OldPosition = { 0.0f,0.0f,0.0f };


	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	float m_Angle = 0.0f;//θ
	float m_Fai = 0.0f;//Φ
	float m_Len = 0.0f;//ターゲットとの距離

	float m_Rot = 0.0f; //向き

	bool m_RotateAroundDoOnce = false;

	const char* m_ShaderVS = "shader\\unlitTextureVS.cso";
	const char* m_ShaderPS = "shader\\unlitTexturePS.cso";

	bool m_OnDraw = true;

public:

	Transform() = default;
	Transform(GameObject* obj) : GameObjectComponent(obj) {}

	virtual void Init() override {};
	virtual void Uninit() override {};
	virtual void Update() override {};
	virtual void Draw() override {};

	XMFLOAT3 GetPosition() const { return m_Position; }
	XMFLOAT3 GetRotation() const { return m_Rotation; }
	XMFLOAT3 GetSize() const { return m_Size; }
	XMFLOAT3 GetVelocity() const { return m_Velocity; }

	float GetLength() const { return m_Len; }

	XMFLOAT3 GetOldPosition() const { return m_OldPosition; }
	float GetRot() const { return m_Rot; }


	void SetPosition(const XMFLOAT3 pos) { m_Position = pos; }
	void SetSize(const XMFLOAT3 size) { m_Size = size; }
	void SetRotation(XMFLOAT3 rot) { m_Rotation = rot; }
	void SetVelocity(XMFLOAT3 velocity) { m_Velocity = velocity; }


	void SetRotationAngle(XMFLOAT3 angle) 
	{

		//角度をラジアンに直す
		float PI = 3.141592;
		angle.x = (PI/180.0f) * angle.x;
		angle.y = (PI / 180.0f) * angle.y;
		angle.z = (PI / 180.0f) * angle.z;


		m_Rotation = angle;
	}


	void SetTransform(const XMFLOAT3 pos, const XMFLOAT3 rot, const XMFLOAT3 size)
	{

		m_Position = pos;
		m_Rotation = rot;
		m_Size = size;

	}

	void SetTransformRotAngle(const XMFLOAT3 pos, const XMFLOAT3 angle, const XMFLOAT3 size)
	{
		XMFLOAT3 Angle{};

		//角度をラジアンに直す
		float PI = 3.141592;
		Angle.x = (PI / 180.0f) * angle.x;
		Angle.y = (PI / 180.0f) * angle.y;
		Angle.z = (PI / 180.0f) * angle.z;

		m_Position = pos;
		m_Rotation = Angle;
		m_Size = size;

	}


	void SetPositionX(const float posX) { m_Position.x = posX; }
	void SetPositionY(const float posY) { m_Position.y = posY; }
	void SetPositionZ(const float posZ) { m_Position.z = posZ; }

	void SetOldPosition(const XMFLOAT3 pos) { m_OldPosition = pos; }
	void SetRot(const float r) { m_Rot = r; }

	void SetShader(const char* vs, const char* ps)
	{

		m_ShaderVS = vs;
		m_ShaderPS = ps;

	}

	void SetLength(float len) { m_Len = len; }
	float GetLength() { return m_Len; }

	void RotateAroundInit(XMFLOAT3 targetPos);
	void RotateAround(float rotateX,float rotateY);
	XMFLOAT3 GetRotatePosition(XMFLOAT3 targetPos) const;


	XMFLOAT3 GetFoward()
	{
		XMMATRIX rotMatrix;
		rotMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 foward;
		XMStoreFloat3(&foward, rotMatrix.r[2]);
		return foward;
	}

	XMFLOAT3 GetRight()
	{
		float PI = 3.141592;

		float addAngle = (PI / 180.0f) * 90.0f;

		XMMATRIX rotMatrix;
		rotMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y + addAngle, m_Rotation.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotMatrix.r[2]);
		return right;
	}

	void SetDraw(bool state) { m_OnDraw = state; }

};