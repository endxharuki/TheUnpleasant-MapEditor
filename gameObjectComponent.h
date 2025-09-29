#pragma once
#include  "component.h"

class GameObject;

class GameObjectComponent : public Component
{
private:

protected:

	////回転行列
	//struct Angle
	//{
	//	XMFLOAT3 AngleX;
	//	XMFLOAT3 AngleY;
	//	XMFLOAT3 AngleZ;

	//};

	GameObject* m_GameObject = nullptr; //親のゲームオブジェクト
	bool isDelete = false;
	int ComponentNumber = 0;
	std::string m_Tag = "";

	


public:

	GameObjectComponent() = default;
	GameObjectComponent(GameObject* Object) : m_GameObject(Object){}

	virtual ~GameObjectComponent() {};

	virtual void Init() override{}
	virtual void Uninit()override {}
	virtual void Update()override {}
	virtual void Draw ()override {}
	virtual void ReInit()override {}

	const int GetComponentNumber()  const  { return ComponentNumber; }
	const std::string GetComponentTag() const  { return m_Tag; }

	void AcceptGameObject(GameObject* obj) {

		m_GameObject = obj;
	 }
	GameObject* GetGameObject(){

		return m_GameObject;
	}
	bool DeleteRecest() {
		if (isDelete) {

			Uninit();
			delete this;
			return true;
		}
		else {
			return false;
		}
	}

	//Angle GetAddAngle(XMFLOAT3 rotation, XMFLOAT3 vector)
	//{
	//
	//	float x = vector.x;
	//	float y = vector.y;
	//	float z = vector.z;

	//	//Pich
	//	float cosP = cosf(rotation.x);
	//	float sinP = sinf(rotation.x);

	//	//Yaw
	//	float cosY = cosf(rotation.y);
	//	float sinY = sinf(rotation.y);

	//	//Rall
	//	float cosR = cosf(rotation.z);
	//	float sinR = sinf(rotation.z);

	//	Angle angle;


	//	angle.AngleX = { x * (cosR * cosY + sinR * sinP * sinY),y * (sinR * cosP),z * (sinR * sinP * cosY - cosR * sinY) };
	//	angle.AngleY = { x * (cosR * sinP * sinY - sinR * cosY),y * (cosR * cosP),z * (sinR * sinY + cosR * sinP * cosY) };
	//	angle.AngleZ = { x * (cosP * sinY),y * (-sinP),z * (cosP * cosY) };

	//	return angle;
	//}

	//XMFLOAT3 GetAddAnglePosition(XMFLOAT3 rotation, XMFLOAT3 vector)
	//{
	//	//Yaw = z
	//	//Pich = y
	//	//Roll = x

	//	float x = vector.x;
	//	float y = vector.y;
	//	float z = vector.z;

	//	//Pich
	//	float cosP = cosf(rotation.x);
	//	float sinP = sinf(rotation.x);

	//	//Yaw
	//	float cosY = cosf(rotation.y);
	//	float sinY = sinf(rotation.y);

	//	//Rall
	//	float cosR = cosf(rotation.z);
	//	float sinR = sinf(rotation.z);

	//	Angle angle;

	//	//{1,0,0}
	//	//{0,1,0}
	//	//{0,0,1}
	//	//合成した回転行列


	//	angle.AngleX = { x * (cosR * cosY + sinR * sinP * sinY),y * (sinR * cosP),z * (sinR * sinP * cosY - cosR * sinY) };
	//	angle.AngleY = { x * (cosR * sinP * sinY - sinR * cosY),y * (cosR * cosP),z * (sinR * sinY + cosR * sinP * cosY) };
	//	angle.AngleZ = { x * (cosP * sinY),y * (-sinP),z * (cosP * cosY) };
	//	XMFLOAT3 position = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z , angle.AngleY.x + angle.AngleY.y + angle.AngleY.z , angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

	//	return position;
	//}

};

