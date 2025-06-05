#pragma once
#include  "component.h"

class GameObject;

class GameObjectComponent : public Component
{
private:

protected:

	//回転行列
	struct Angle
	{
		XMFLOAT3 AngleX;
		XMFLOAT3 AngleY;
		XMFLOAT3 AngleZ;

	};

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

	float Dot(XMFLOAT3 vec1, XMFLOAT3 vec2) const {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	float LengthSquared(const XMFLOAT3 vec) const
	{
		return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
	}
	float Length(const XMFLOAT3 vec) const
	{
		return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	}
	XMFLOAT3 Cross(const XMFLOAT3 v1, const XMFLOAT3 v2)
	{
		return XMFLOAT3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}

	XMFLOAT3 Normalize(XMFLOAT3 vec)
	{
		float len = Length(vec);

		if (len != 0.0f)
		{
			return XMFLOAT3(vec.x / len, vec.y / len, vec.z / len);
		}
		return XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	float Lerp(float vector, float target, float speed)
	{
		return vector + (target - vector) * speed;
	}

	XMFLOAT3 Sub(XMFLOAT3 vec1, XMFLOAT3 vec2)
	{
		return{ vec1.x - vec2.x,vec1.y - vec2.y,vec1.z - vec2.z };
	}
	XMFLOAT3 Add(XMFLOAT3 vec1, XMFLOAT3 vec2)
	{
		return{ vec1.x + vec2.x,vec1.y + vec2.y,vec1.z + vec2.z };
	}
	XMFLOAT3 Mul(XMFLOAT3 vec1, XMFLOAT3 vec2)
	{
		return{ vec1.x * vec2.x,vec1.y * vec2.y,vec1.z * vec2.z };
	}

	XMFLOAT3 MulFloat(XMFLOAT3 vec, float f)
	{
		return { vec.x * f,vec.y * f,vec.z * f };
	}


	Angle GetAddAngle(XMFLOAT3 rotation, XMFLOAT3 vector)
	{
		//Yaw = z
		//Pich = y
		//Roll = x

		float x = vector.x;
		float y = vector.y;
		float z = vector.z;

		//Pich
		float cosP = cosf(rotation.x);
		float sinP = sinf(rotation.x);

		//Yaw
		float cosY = cosf(rotation.y);
		float sinY = sinf(rotation.y);

		//Rall
		float cosR = cosf(rotation.z);
		float sinR = sinf(rotation.z);

		Angle angle;

		//DirectX の回転行列(数学の公式とPichとRallのの軸と、合成の計算が違うから気を付けて)
		/*M.m[0][0] = cr * cy + sr * sp * sy;
		M.m[0][1] = sr * cp;
		M.m[0][2] = sr * sp * cy - cr * sy;
		M.m[0][3] = 0.0f;

		M.m[1][0] = cr * sp * sy - sr * cy;
		M.m[1][1] = cr * cp;
		M.m[1][2] = sr * sy + cr * sp * cy;
		M.m[1][3] = 0.0f;

		M.m[2][0] = cp * sy;
		M.m[2][1] = -sp;
		M.m[2][2] = cp * cy;
		M.m[2][3] = 0.0f;*/


		//各軸のクオータニオンを算出
		//クオータニオンの合成
		//XMVECTOR v = XMQuaternionRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		//マトリックスに変換(ワールド座標に)
		//XMMATRIX mat = XMMatrixRotationQuaternion(v);


		angle.AngleX = { x * (cosR * cosY + sinR * sinP * sinY),y * (sinR * cosP),z * (sinR * sinP * cosY - cosR * sinY) };
		angle.AngleY = { x * (cosR * sinP * sinY - sinR * cosY),y * (cosR * cosP),z * (sinR * sinY + cosR * sinP * cosY) };
		angle.AngleZ = { x * (cosP * sinY),y * (-sinP),z * (cosP * cosY) };
		//angle.AngleX = { mat.r[0].m128_f32[0],mat.r[0].m128_f32[1] ,mat.r[0].m128_f32[2] };
		//angle.AngleY = { mat.r[1].m128_f32[0],mat.r[1].m128_f32[1] ,mat.r[1].m128_f32[2] };
		//angle.AngleZ = { mat.r[2].m128_f32[0],mat.r[2].m128_f32[1] ,mat.r[2].m128_f32[2] };


		return angle;
	}

	XMFLOAT3 GetAddAnglePosition(XMFLOAT3 rotation, XMFLOAT3 vector)
	{
		//Yaw = z
		//Pich = y
		//Roll = x

		float x = vector.x;
		float y = vector.y;
		float z = vector.z;

		//Pich
		float cosP = cosf(rotation.x);
		float sinP = sinf(rotation.x);

		//Yaw
		float cosY = cosf(rotation.y);
		float sinY = sinf(rotation.y);

		//Rall
		float cosR = cosf(rotation.z);
		float sinR = sinf(rotation.z);

		Angle angle;

		//{1,0,0}
		//{0,1,0}
		//{0,0,1}
		//合成した回転行列


		angle.AngleX = { x * (cosR * cosY + sinR * sinP * sinY),y * (sinR * cosP),z * (sinR * sinP * cosY - cosR * sinY) };
		angle.AngleY = { x * (cosR * sinP * sinY - sinR * cosY),y * (cosR * cosP),z * (sinR * sinY + cosR * sinP * cosY) };
		angle.AngleZ = { x * (cosP * sinY),y * (-sinP),z * (cosP * cosY) };
		XMFLOAT3 position = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z , angle.AngleY.x + angle.AngleY.y + angle.AngleY.z , angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

		return position;
	}

};

