#include "main.h"
#include "scene.h"
#include "gameScene.h"
#include "transform3DComponent.h"
#include "transform.h"

void Transform::RotateAroundInit(XMFLOAT3 targetPos)
{
	//ターゲットのポジションを見て球座標と距離を求める（一回だけ）
	if (m_RotateAroundDoOnce == false)
	{
		float vx = m_Position.x - targetPos.x;
		float vy = m_Position.y - targetPos.y;
		float vz = m_Position.z - targetPos.z;

		float x2 = vx * vx;
		float y2 = vy * vy;
		float z2 = vz * vz;

		//球座標を求める
		m_Len = sqrt(x2 + y2 + z2);
		m_Angle = atan(vz / vx);
		m_Fai = atan(sqrt(x2 + z2) / vy);
		m_RotateAroundDoOnce = true;
	}

}

void Transform::RotateAround(float rotateX, float rotateY)
{
	
	m_Angle += rotateX;
	m_Fai += rotateY;
	m_Rotation.y -= rotateX;

}

XMFLOAT3 Transform::GetRotatePosition(XMFLOAT3 targetPos) const
{
	XMFLOAT3 rotatePosition = m_Position;

	if (m_RotateAroundDoOnce == true) {
		rotatePosition.x = (m_Len * sin(m_Fai) * cos(m_Angle)) + targetPos.x;
		rotatePosition.y = (m_Len * cos(m_Fai)) + targetPos.y;
		rotatePosition.z = (m_Len * sin(m_Fai) * sin(m_Angle)) + targetPos.z;
	}

	return rotatePosition;
}
