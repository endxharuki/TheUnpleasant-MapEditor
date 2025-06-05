#include "main.h"
#include "scene.h"
#include "gameScene.h"
#include "transform3DComponent.h"
#include "colider.h"
#include "boxColiderComponent.h"
#include "capsuleColiderComponent.h"

void Colider::RotateAroundInit(XMFLOAT3 targetPos)
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
		m_Angle = atanf(vz / vx);
		m_Fai = atanf(sqrt(x2 + z2) / vy);
		if (vx <= -0.001f)
		{
			m_Angle = 3.141592 + m_Angle;
		}

		m_RotateAroundDoOnce = true;
	}

}

void Colider::RotateAround(float rotateX, float rotateY)
{

	m_Angle += rotateX;
	m_Fai += rotateY;
	m_Rotation.y -= rotateX;

}

XMFLOAT3 Colider::GetRotatePosition(XMFLOAT3 targetPos) const
{
	XMFLOAT3 rotatePosition = m_Position;

	if (m_RotateAroundDoOnce == true) {
		rotatePosition.x = (m_Len * sin(m_Fai) * cos(m_Angle)) + targetPos.x;
		rotatePosition.y = (m_Len * cos(m_Fai)) + targetPos.y;
		rotatePosition.z = (m_Len * sin(m_Fai) * sin(m_Angle)) + targetPos.z;
	}

	if (std::abs(rotatePosition.x) <= 0.00001f) rotatePosition.x = 0.0f;
	if (std::abs(rotatePosition.y) <= 0.00001f) rotatePosition.y = 0.0f;
	if (std::abs(rotatePosition.z) <= 0.00001f) rotatePosition.z = 0.0f;


	return rotatePosition;
}

XMFLOAT3 Colider::clampToBox(const XMFLOAT3 point, const XMFLOAT3 halfSize)
{
	return {
			std::max(-halfSize.x, std::min(point.x, halfSize.x)),
			std::max(-halfSize.y, std::min(point.y, halfSize.y)),
			std::max(-halfSize.z, std::min(point.z, halfSize.z))
	};
}

bool Colider::BoxsCollision(Box obb1, Box obb2)
{
	XMFLOAT3 distance = { obb2.center.x - obb1.center.x,obb2.center.y - obb1.center.y,obb2.center.z - obb1.center.z };


	for (int i = 0; i < 3; i++)
	{


		float r1 = obb1.halfSize.x * std::fabs(Dot(obb1.axis[i], obb1.axis[0])) +
			obb1.halfSize.y * std::fabs(Dot(obb1.axis[i], obb1.axis[1])) +
			obb1.halfSize.z * std::fabs(Dot(obb1.axis[i], obb1.axis[2]));

		float r2 = obb2.halfSize.x * std::fabs(Dot(obb1.axis[i], obb2.axis[0])) +
			obb2.halfSize.y * std::fabs(Dot(obb1.axis[i], obb2.axis[1])) +
			obb2.halfSize.z * std::fabs(Dot(obb1.axis[i], obb2.axis[2]));

		float distanceOnAxis = std::fabs(Dot(distance, obb1.axis[i]));

		if (distanceOnAxis > r1 + r2)
		{
			return false;
		}
	}

	for (int i = 0; i < 3; i++)
	{

		XMFLOAT3 distance = { obb2.center.x - obb1.center.x,obb2.center.y - obb1.center.y,obb2.center.z - obb1.center.z };

		float r1 = obb1.halfSize.x * std::fabs(Dot(obb2.axis[i], obb1.axis[0])) +
			obb1.halfSize.y * std::fabs(Dot(obb2.axis[i], obb1.axis[1])) +
			obb1.halfSize.z * std::fabs(Dot(obb2.axis[i], obb1.axis[2]));

		float r2 = obb2.halfSize.x * std::fabs(Dot(obb2.axis[i], obb2.axis[0])) +
			obb2.halfSize.y * std::fabs(Dot(obb2.axis[i], obb2.axis[1])) +
			obb2.halfSize.z * std::fabs(Dot(obb2.axis[i], obb2.axis[2]));

		float distanceOnAxis = std::fabs(Dot(distance, obb2.axis[i]));

		if (distanceOnAxis > r1 + r2)
		{
			return false;
		}
	}

	return true;

}

float Colider::SegmentDistance(const XMFLOAT3 vec1start, const XMFLOAT3 vec1end, const XMFLOAT3 vec2start, const XMFLOAT3 vec2end, float& t1, float& t2)
{
	XMFLOAT3 d1 = { vec1end.x - vec1start.x, vec1end.y - vec1start.y ,vec1end.z - vec1start.z };
	XMFLOAT3 d2 = { vec2end.x - vec2start.x, vec2end.y - vec2start.y ,vec2end.z - vec2start.z };
	XMFLOAT3 r = { vec1start.x - vec2start.x, vec1start.y - vec2start.y ,vec1start.z - vec2start.z };


	float a = Dot(d1, d1); //線部1の長さの2乗
	float e = Dot(d2, d2); //線部2の長さの2乗
	//float b = Dot(d1, d2);
	//float c = Dot(d1, r);
	float f = Dot(d2, r);


	float s = 0.0f, t = 0.0f;
	//float denom = a * e - b * b;
	/*if (std::abs(denom) > 1e-6f)
	{
		s = (b * f - c * e) / denom;
		t = (a * f - b * c) / denom;
	}*/

	if (a > 1e-6f && e > 1e-6)//値が小さすぎなかったら
	{
		float b = Dot(d1, d2);
		float c = Dot(d1, r);
		float denom = a * e - b * b;

		if (std::abs(denom) > 1e-6f)//線分同士が平行じゃない
		{

			s = (b * f - c * e) / denom;
			t = (a * f - b * c) / denom;
		}

		s = std::clamp(s, 0.0f, 1.0f);
		t = std::clamp(t, 0.0f, 1.0f);
	}

	t1 = s;
	t2 = t;

	XMFLOAT3 closestPoint1 = { d1.x * s,d1.y * s,d1.z * s };
	closestPoint1 = { vec1start.x + closestPoint1.x,vec1start.y + closestPoint1.y ,vec1start.z + closestPoint1.z };

	XMFLOAT3 closestPoint2 = { d2.x * t,d2.y * t,d2.z * t };
	closestPoint2 = { vec2start.x + closestPoint2.x, vec2start.y + closestPoint2.y ,vec2start.z + closestPoint2.z };

	//clossPoint1 = closestPoint1;
	//clossPoint2 = closestPoint2;

	XMFLOAT3 length = { closestPoint1.x - closestPoint2.x,closestPoint1.y - closestPoint2.y ,closestPoint1.z - closestPoint2.z };

	return LengthSquared(length);
}

bool Colider::CapsulesCollision(GameObject* capsuleColider1, GameObject* capsuleColider2)
{
	Capsule capsule1 = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule();
	Capsule capsule2 = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule();


	float t1, t2;
	float distanceSquared = SegmentDistance(capsule1.startPos, capsule1.endPos, capsule2.startPos, capsule2.endPos, t1, t2);

	float radiusSum = capsule1.radius + capsule2.radius;

	if (distanceSquared <= radiusSum * radiusSum)
	{
		if (capsuleColider1->GetTag() == "Attack") return true;
		if (capsuleColider2->GetTag() == "Attack") return true;

		XMFLOAT3 cap1Len = { capsule1.endPos.x - capsule1.startPos.x, capsule1.endPos.y - capsule1.startPos.y ,capsule1.endPos.z - capsule1.startPos.z };
		XMFLOAT3 cap2Len = { capsule2.endPos.x - capsule2.startPos.x, capsule2.endPos.y - capsule2.startPos.y ,capsule2.endPos.z - capsule2.startPos.z };

		cap1Len = { cap1Len.x * t1,cap1Len.y * t1 ,cap1Len.z * t1 };
		cap1Len = { cap2Len.x * t2,cap2Len.y * t2 ,cap2Len.z * t2 };


		// 衝突時の最近点と法線計算
		XMFLOAT3 point1 = { capsule1.startPos.x + cap1Len.x,capsule1.startPos.y + cap1Len.y ,capsule1.startPos.z + cap1Len.z };
		XMFLOAT3 point2 = { capsule2.startPos.x + cap2Len.x,capsule2.startPos.y + cap2Len.y ,capsule2.startPos.z + cap2Len.z };

		XMFLOAT3 pointLen = { point1.x - point2.x,point1.y - point2.y ,point1.z - point2.z };

		XMFLOAT3 normal = Normalize(pointLen);

		//埋まっている深さを計算
		float penetrationDepth = radiusSum - sqrt(distanceSquared);

		XMFLOAT3 obj1Pos = capsuleColider1->GetComponent<Colider>()->GetPosition();
		XMFLOAT3 obj2Pos = capsuleColider2->GetComponent<Colider>()->GetPosition();

		XMFLOAT3 vectorObjects = { obj1Pos.x - obj2Pos.x,0.0f ,obj1Pos.z - obj2Pos.z };

		vectorObjects = Normalize(vectorObjects);

		//修正ベクトル
		XMFLOAT3 correction = { vectorObjects.x * penetrationDepth,vectorObjects.y * penetrationDepth,vectorObjects.z * penetrationDepth };
		/*XMFLOAT3 correction = MulFloat(normal, penetrationDepth);*/
		if (Length(correction) <= 0.00001f)
		{
			return false;
		}


		//位置を補正
		//当たったオブジェクトが静的だったら
		if (capsuleColider2->GetObjectKind() == Static)
		{
			XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
			XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;

			CapsuleColiderComponent* col = capsuleColider1->GetComponent<CapsuleColiderComponent>();

			col->SetCapsuleStartPos(Add(myStartPos, correction));
			col->SetCapsuleStartPos(Add(myEndPos, correction));
			m_Position = Add(m_Position, correction);

			m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);
		}
		//当たったオブジェクトが動的だったら
		else if (capsuleColider2->GetObjectKind() == Dynamic)
		{

			int weight1 = capsuleColider1->GetWeight();
			int weight2 = capsuleColider2->GetWeight();


			//当たったオブジェクトの重さが自分よりも大きかったら
			if (weight2 > weight1)
			{
				//自分を動かす
				XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
				XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;

				CapsuleColiderComponent* col1 = capsuleColider1->GetComponent<CapsuleColiderComponent>();
				Transform* trans1 = capsuleColider1->GetComponent<Transform>();
				XMFLOAT3 trans1Pos = trans1->GetPosition();

				col1->SetCapsuleStartPos(Add(myStartPos, correction));
				col1->SetCapsuleStartPos(Add(myEndPos, correction));

				m_Position = Add(m_Position, correction);
				trans1->SetPosition(Add(trans1Pos, correction));
			}
			//当たったオブジェクトの重さが自分よりも小さかったら
			else if (weight2 < weight1)
			{
				//相手を動かす
				CapsuleColiderComponent* col2 = capsuleColider2->GetComponent<CapsuleColiderComponent>();
				Transform* trans2 = capsuleColider2->GetComponent<Transform>();

				XMFLOAT3 otherPos = col2->GetPosition();
				XMFLOAT3 otherStartPos = col2->GetCapsule().startPos;
				XMFLOAT3 otherEndPos = col2->GetCapsule().endPos;
				XMFLOAT3 trans2Pos = trans2->GetPosition();

				col2->SetCapsuleStartPos(Sub(otherStartPos, correction));
				col2->SetCapsuleStartPos(Sub(otherEndPos, correction));

				col2->SetPosition(Sub(otherPos, correction));
				trans2->SetPosition(Sub(trans2Pos, correction));
			}
			//当たったオブジェクトの重さが自分と同じだったら
			else if (weight2 == weight1)
			{
				//自分を動かす
				XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
				XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;

				CapsuleColiderComponent* col1 = capsuleColider1->GetComponent<CapsuleColiderComponent>();
				Transform* trans1 = capsuleColider1->GetComponent<Transform>();
				XMFLOAT3 trans1Pos = trans1->GetPosition();

				col1->SetCapsuleStartPos(Add(myStartPos, correction));
				col1->SetCapsuleStartPos(Add(myEndPos, correction));
			
				m_Position = Add(m_Position, correction);
				trans1->SetPosition(Add(trans1Pos, correction));

				//相手を動かす
				CapsuleColiderComponent* col2 = capsuleColider2->GetComponent<CapsuleColiderComponent>();
				Transform* trans2 = capsuleColider2->GetComponent<Transform>();

				XMFLOAT3 otherPos = col2->GetPosition();
				XMFLOAT3 otherStartPos = col2->GetCapsule().startPos;
				XMFLOAT3 otherEndPos = col2->GetCapsule().endPos;
				XMFLOAT3 trans2Pos = trans2->GetPosition();

				col2->SetCapsuleStartPos(Sub(otherStartPos, correction));
				col2->SetCapsuleStartPos(Sub(otherEndPos, correction));

				col2->SetPosition(Sub(otherPos, correction));
				trans2->SetPosition(Sub(trans2Pos, correction));


				////自分を動かす
				//XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
				//XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;

				//correction = MulFloat(correction, 0.5f);

				//CapsuleColiderComponent* col1 = capsuleColider1->GetComponent<CapsuleColiderComponent>();

				//col1->SetCapsuleStartPos(Add(myStartPos, correction));
				//col1->SetCapsuleStartPos(Add(myEndPos, correction));
				//m_Position = Add(m_Position, correction);

				//m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);
				////相手を動かす
				//XMFLOAT3 otherPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetPosition();
				//XMFLOAT3 otherStartPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
				//XMFLOAT3 otherEndPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
				//CapsuleColiderComponent* col2 = capsuleColider2->GetComponent<CapsuleColiderComponent>();


				//col2->SetCapsuleStartPos(Sub(otherStartPos, correction));
				//col2->SetCapsuleStartPos(Sub(otherEndPos, correction));

				//XMFLOAT3 pos = Sub(otherPos, correction);
				//col2->SetPosition(pos);
				//capsuleColider2->GetComponent<Transform>()->SetPosition(pos);
			}
		}


		return true;

	}

	return false;

}

bool Colider::CapsuleBoxCollision(GameObject* capsuleObj, GameObject* boxObj)
{
	const float MIN = 0.00001f; //無視する値

	const int MAX_COUNT = 5;//最大処理回数

	Box box = boxObj->GetComponent<BoxColiderComponent>()->GetOBB();

	Angle angle;

	Capsule capsule = capsuleObj->GetComponent<CapsuleColiderComponent>()->GetCapsule();

	int count = 0;
	while (count < MAX_COUNT)
	{
		// カプセルのローカル座標変換
		XMFLOAT3 localStart = ToLocalSpace(capsule.startPos, box);
		XMFLOAT3 localEnd = ToLocalSpace(capsule.endPos, box);

		// ボックスの範囲内にクランプ
		XMFLOAT3 clampedStart = clampToBox(localStart, box.halfSize);
		XMFLOAT3 clampedEnd = clampToBox(localEnd, box.halfSize);

		// 衝突判定（始点または終点が埋まっている場合）
		float distStartSquared = Dot(Sub(localStart, clampedStart), Sub(localStart, clampedStart));
		float distEndSquared = Dot(Sub(localEnd, clampedEnd), Sub(localEnd, clampedEnd));

		//衝突判定
		if (distStartSquared > capsule.radius * capsule.radius && distEndSquared > capsule.radius * capsule.radius) {
			return count > 0; // 一度でも当たったらtrueを返す
		}

		//一番深いところのポジション
		XMFLOAT3 DeepPoint;

		//埋まった深さの線分の始点と終点
		XMFLOAT3 penetrationStart;
		XMFLOAT3 penetrationEnd;


		if (distStartSquared > distEndSquared)
		{
			DeepPoint = clampedEnd;// 最も深く埋まったボックス側の点
			penetrationStart = localEnd;// カプセル側の点（終点）
			penetrationEnd = clampedEnd;// ボックス側の点
		}
		else
		{
			DeepPoint = clampedStart;// 最も深く埋まったボックス側の点
			penetrationStart = localStart;// カプセル側の点（始点）
			penetrationEnd = clampedStart;// ボックス側の点
		}

		XMFLOAT3 penetrationVector = Sub(penetrationStart, penetrationEnd);


		// 埋まり深さを計算
		float penetrationDepth = capsule.radius - Length(penetrationVector);
		if (penetrationDepth <= MIN) {
			break; // 埋まりがない場合処理を終了
		}

		//カプセルの法線
		XMFLOAT3 localNormal = Normalize(penetrationVector);
		//当たったボックスの面の法線
		XMFLOAT3 worldNormal = Sub(ToWorldSpace(localNormal, box), box.center);

		XMFLOAT3 correction = MulFloat(Normalize(worldNormal), penetrationDepth);

		DeepPoint = ToWorldSpace(DeepPoint, box);
		penetrationStart = ToWorldSpace(penetrationStart, box);
		penetrationEnd = ToWorldSpace(penetrationEnd, box);

		float PI = 3.14159265359 / 2;
		/*(PI / 180.0f)* angle.x;*/
		XMFLOAT3 t = { 0.0f,1.0f,0.0f };

		float DotProdct = Dot(t, worldNormal);
		float len = Length(t) * Length(worldNormal);

		float cosineTheta = DotProdct / len;

		cosineTheta = std::fmax(-1.0f, std::fmin(1.0f, cosineTheta));

		//二つのベクトルのなす角
		float radian = std::acos(cosineTheta);

		XMVECTOR deep = { DeepPoint.x,DeepPoint.y,DeepPoint.z,0.0f };
		XMVECTOR cE = { clampedEnd.x,clampedEnd.y ,clampedEnd.z ,0.0f };

		//法線と真上のベクトルのなす角が50度より小さかったら
		if (radian <= 50.0f * (3.14159265359 / 180.0f) && penetrationStart.y <= capsule.endPos.y + m_Size.x)
		{

			float a = 90.0f;
			float b = 90.0f - (radian / ((2 * PI) / 180.0f));
			float c = 180.0f - (a + b);

			//半径
			float r = (penetrationDepth / sinf(a)) / 2;

			float A = (2 * r) * sinf(a);//aの角の対の辺
			float B = (2 * r) * sinf(b);//bの角の対の辺
			float C = (2 * r) * sinf(c);//今わかっている長さ

			float baiA = sqrt((B * B) + (C * C));

			float ratioA = A / C;

			float reA = ratioA * penetrationDepth;

			//進んでる方向の逆ベクトル
			XMFLOAT3 reverseVelocity = { 0.0f,worldNormal.y,0.0f };
			XMFLOAT3 reverseVector = Normalize(reverseVelocity);

			reverseVector = { reverseVector.x * baiA,reverseVector.y * baiA,reverseVector.z * baiA };

			/*m_Position = Add(m_Position, reverseVector);*/
			//埋まりを解消
			//当たったオブジェクトが静的だったら
			if (boxObj->GetObjectKind() == Static)
			{
				capsule.startPos = Add(capsule.startPos, reverseVector);
				capsule.endPos = Add(capsule.endPos, reverseVector);
				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
			}
			//当たったオブジェクトが動的だったら
			else if (boxObj->GetObjectKind() == Dynamic)
			{
				capsule.startPos = Add(capsule.startPos, reverseVector);
				capsule.endPos = Add(capsule.endPos, reverseVector);
				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
			}

			//オブジェクトに乗っているオブジェクトの重力をoff
			m_GameObject->SetGravityState(false);
			m_GameObject->SetGravitiScale(0.05f);

		}
		else
		{

			float a = 90.0f;
			float b = 90.0f - (radian / ((2 * PI) / 180.0f));
			float c = 180.0f - (a + b);

			//半径
			float r = (penetrationDepth / sinf(a)) / 2;

			float A = (2 * r) * sinf(a);//aの角の対の辺
			float B = (2 * r) * sinf(b);//bの角の対の辺
			float C = (2 * r) * sinf(c);//今わかっている長さ

			float baiA = sqrt((B * B) + (C * C));

			//法線ベクトル
			XMFLOAT3 reverseVelocity = { worldNormal.x,0.0f,worldNormal.z };
			XMFLOAT3 reverseVector = Normalize(reverseVelocity);

			reverseVector = { reverseVector.x * baiA,reverseVector.y * baiA,reverseVector.z * baiA };

			/*m_Position = Add(m_Position, reverseVector);*/
			//当たったオブジェクトが静的だったら
			if (boxObj->GetObjectKind() == Static)
			{
				capsule.startPos = Add(capsule.startPos, reverseVector);
				capsule.endPos = Add(capsule.endPos, reverseVector);
				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
			}
			//当たったオブジェクトが動的だったら
			else if (boxObj->GetObjectKind() == Dynamic)
			{
				//当たったオブジェクトの重さが自分よりも大きかったら
				if (boxObj->GetWeight() > m_GameObject->GetWeight())
				{
					capsule.startPos = Add(capsule.startPos, reverseVector);
					capsule.endPos = Add(capsule.endPos, reverseVector);
					XMFLOAT3 Pos = boxObj->GetComponent<Colider>()->GetPosition();
					Pos = Add(Pos, reverseVector);

					boxObj->GetComponent<Colider>()->SetPosition(Pos);
					boxObj->GetComponent<Transform>()->SetPosition(Pos);
				}
				//当たったオブジェクトの重さが自分よりも小さかったら
				else if (boxObj->GetWeight() < m_GameObject->GetWeight())
				{
					XMFLOAT3 Pos = boxObj->GetComponent<Colider>()->GetPosition();
					Pos = Sub(Pos, reverseVector);

					boxObj->GetComponent<Colider>()->SetPosition(Pos);
					boxObj->GetComponent<Transform>()->SetPosition(Pos);
				}
				//当たったオブジェクトの重さが自分と同じだったら
				else if (boxObj->GetWeight() == m_GameObject->GetWeight())
				{
					//自分を動かす
					capsule.startPos = Add(capsule.startPos, MulFloat(reverseVector, 0.5f));
					capsule.endPos = Add(capsule.endPos, MulFloat(reverseVector, 0.5f));
					XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
					m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, MulFloat(reverseVector, 0.5f)));

					//相手を動かす
					XMFLOAT3 Pos = boxObj->GetComponent<Colider>()->GetPosition();
					Pos = Sub(Pos, MulFloat(reverseVector, 0.5f));

					boxObj->GetComponent<Colider>()->SetPosition(Pos);
					boxObj->GetComponent<Transform>()->SetPosition(Pos);
				}
			}

		}
		count++;
	}
}




