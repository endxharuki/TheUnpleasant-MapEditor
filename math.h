#pragma once
#include "main.h"

//âÒì]çsóÒ
struct Angle
{
	XMFLOAT3 AngleX;
	XMFLOAT3 AngleY;
	XMFLOAT3 AngleZ;

};

constexpr auto PI = (3.14159265359);

inline float Dot(XMFLOAT3 vec1, XMFLOAT3 vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

inline float LengthSquared(const XMFLOAT3 vec)
{
	return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
}
inline float Length(const XMFLOAT3 vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}
inline XMFLOAT3 Cross(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	return XMFLOAT3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

inline XMFLOAT3 Normalize(XMFLOAT3 vec)
{
	float len = Length(vec);

	if (len != 0.0f)
	{
		return XMFLOAT3(vec.x / len, vec.y / len, vec.z / len);
	}
	return XMFLOAT3(0.0f, 0.0f, 0.0f);
}

inline float Lerp(float vector, float target, float speed)
{
	return vector + (target - vector) * speed;
}

inline XMFLOAT3 Sub(XMFLOAT3 vec1, XMFLOAT3 vec2)
{
	return{ vec1.x - vec2.x,vec1.y - vec2.y,vec1.z - vec2.z };
}
inline XMFLOAT3 Add(XMFLOAT3 vec1, XMFLOAT3 vec2)
{
	return{ vec1.x + vec2.x,vec1.y + vec2.y,vec1.z + vec2.z };
}
inline XMFLOAT3 Mul(XMFLOAT3 vec1, XMFLOAT3 vec2)
{
	return{ vec1.x * vec2.x,vec1.y * vec2.y,vec1.z * vec2.z };
}

inline XMFLOAT3 MulFloat(XMFLOAT3 vec, float f)
{
	return { vec.x * f,vec.y * f,vec.z * f };
}

inline Angle GetAddAngle(XMFLOAT3 rotation, XMFLOAT3 vector)
{

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


	angle.AngleX = { x * (cosR * cosY + sinR * sinP * sinY),y * (sinR * cosP),z * (sinR * sinP * cosY - cosR * sinY) };
	angle.AngleY = { x * (cosR * sinP * sinY - sinR * cosY),y * (cosR * cosP),z * (sinR * sinY + cosR * sinP * cosY) };
	angle.AngleZ = { x * (cosP * sinY),y * (-sinP),z * (cosP * cosY) };

	return angle;
}

inline XMFLOAT3 GetAddAnglePosition(XMFLOAT3 rotation, XMFLOAT3 vector)
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
	//çáê¨ÇµÇΩâÒì]çsóÒ


	angle.AngleX = { x * (cosR * cosY + sinR * sinP * sinY),y * (sinR * cosP),z * (sinR * sinP * cosY - cosR * sinY) };
	angle.AngleY = { x * (cosR * sinP * sinY - sinR * cosY),y * (cosR * cosP),z * (sinR * sinY + cosR * sinP * cosY) };
	angle.AngleZ = { x * (cosP * sinY),y * (-sinP),z * (cosP * cosY) };
	XMFLOAT3 position = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z , angle.AngleY.x + angle.AngleY.y + angle.AngleY.z , angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

	return position;
}

