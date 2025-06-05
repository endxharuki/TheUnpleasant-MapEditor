#include "main.h"
#include "capsuleColiderComponent.h"
#include "gameScene.h"
#include "scene.h"
#include "transform2DComponent.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"


/*
座標A = 動かす前の元の座標
座標B = 動かした後の座標
/*
* AはPosition
x軸回転
Bx = Ax * 1 + Ay * 0 + Az * 0,
By = Ax * 0 + Ay * cosβ + Az * sinβ
Bz = Ax * 0 + Ay * -sinβ + Az * cosβ
y軸回転
Bx = Ax * cosβ + Ay * 0 + Az * sinβ,
By = Ax * 0 + Ay * 1 + Az * 0
Bz = Ax * -sinβ + Ay * 0 + Az * cosβ
z軸回転
Bx = Ax * cosβ + Ay * sinβ + Az * 0,
By = Ax * -sinβ + Ay * cosβ + Az * 0
Bz = Ax * 0 + Ay * 0 + Az * 1
*/

void CapsuleColiderComponent::Init()
{
	m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
	m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();

	m_ColiderKind = CAPSULE;

	transform = new Transform3DComponent();

	transform->Init();
	transform->AddModelData("asset\\Model\\capsule.obj");

}

void CapsuleColiderComponent::Uninit()
{
	transform->Uninit();
	delete transform;
}

void CapsuleColiderComponent::Update()
{

	

	
	////直交座標の回転行列
	//{
	//	Angle angle;
	//	// 最初　0,m_CenterPosLength,0になるくね
	//	//m_Positionを引けば良い
	//	XMFLOAT3 vec;
	//	XMFLOAT3 topVec;
	//	XMFLOAT3 bottomVec;

	//	
	//	vec = { 0.0f,1.0f,0.0f };
	//	angle = GetAddAngle(m_Rotation, vec);

	//	topVec.x = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z };
	//	topVec.y = { angle.AngleY.x + angle.AngleY.y + angle.AngleY.z };
	//	topVec.z = { angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

	//	m_StartPosition.x = topVec.x + m_Position.x;
	//	m_StartPosition.y = topVec.y + m_Position.y;
	//	m_StartPosition.z = topVec.z + m_Position.z;

	//	vec = { 0.0f,-1.0f,0.0f };

	//	angle = GetAddAngle(m_Rotation, vec);

	//	bottomVec.x = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z };
	//	bottomVec.y = { angle.AngleY.x + angle.AngleY.y + angle.AngleY.z };
	//	bottomVec.z = { angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

	//	m_EndPosition.x = bottomVec.x + m_Position.x;
	//	m_EndPosition.y = bottomVec.y + m_Position.y;
	//	m_EndPosition.z = bottomVec.z + m_Position.z;


	//}
	if (update == true) {

		m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
		/*m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();*/
		m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();

		MoveCollision();

		std::tuple<bool, GameObject*, std::list<GameObject*>> list = GetCollision();

		m_IsCollision = std::get<0>(list);

	}
	if (DrawColider == true)
	{

		transform->SetSize(XMFLOAT3(m_Size.x, m_CenterPosLngth, m_Size.z));
		transform->SetPosition(m_Position);
		transform->SetRotation(m_Rotation);

		transform->Update();
	}
	/*m_Cupsule =
	{
		m_StartPosition,
		m_EndPosition,
		m_Size.x
	};*/

	/*rotatePosition.x = (m_CenterPosLngth * sin(m_Fai) * cos(m_Angle)) + targetPos.x;
    rotatePosition.y = (m_CenterPosLngth * cos(m_Fai)) + targetPos.y;
    rotatePosition.z = (m_CenterPosLngth * sin(m_Fai) * sin(m_Angle)) + targetPos.z;*/

}

void CapsuleColiderComponent::Draw()
{

	if (DrawColider == true)
	{
		transform->Draw();
	}
	DrawImGui();

}


//カプセルボックス
float boxCupsuleLengthStart = 0.0f;
float boxCupsuleLengthEnd = 0.0f;
XMFLOAT3 end;
XMFLOAT3 start;

void CapsuleColiderComponent::DrawImGui()
{

	{
		if (onImGui == true) {
			ImGui::Begin("CapsuleToCapsule");                          // Create a window called "Hello, world!" and append into it.


			ImGui::Text("CupsulePos , x = %.1f , y = %.1f , z = %.1f", m_Position.x, m_Position.y, m_Position.z);
			ImGui::Text("StartPoint , x = %.1f , y = %.1f , z = %.1f", m_Capsule.startPos.x, m_Capsule.startPos.y, m_Capsule.startPos.z);
			ImGui::Text("EndPoint , x = %.1f , y = %.1f , z = %.1f", m_Capsule.endPos.x, m_Capsule.endPos.y, m_Capsule.endPos.z);

			ImGui::Text("ClossPoint1 ,x = %.1f , y = %.1f , z = %.1f", clossPoint1.x, clossPoint1.y, clossPoint1.z);
			ImGui::Text("ClossPoint2 ,x = %.1f , y = %.1f , z = %.1f", clossPoint2.x, clossPoint2.y, clossPoint2.z);

			ImGui::Text("Lenght = %.1f", m_length);

			ImGui::End();

			ImGui::Begin("CapsuleToBox");

			ImGui::Text("LenghtStartPoint = %.1f", boxCupsuleLengthStart);
			ImGui::Text("LenghtEndPoint = %.1f", boxCupsuleLengthEnd);

			ImGui::Text("ClosestStart , x = %.1f , y = %.1f , z = %.1f", start.x, start.y, start.z);
			ImGui::Text("ClosestEnd , x = %.1f , y = %.1f , z = %.1f", end.x, end.y, end.z);


			ImGui::End();

		}
	}


}

void CapsuleColiderComponent::MoveCollision()
{

	m_Position = Add(m_Position, m_AddPosition);

	//直交座標の回転行列
	{
		Angle angle;
		// 最初　0,m_CenterPosLength,0になるくね
		//m_Positionを引けば良い
		XMFLOAT3 vec;
		XMFLOAT3 topVec;
		XMFLOAT3 bottomVec;


		vec = { 0.0f,1.0f,0.0f };
		angle = GetAddAngle(m_Rotation, vec);

		topVec.x = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z };
		topVec.y = { angle.AngleY.x + angle.AngleY.y + angle.AngleY.z };
		topVec.z = { angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

		topVec = MulFloat(topVec, m_CenterPosLngth);



		m_Capsule.startPos.x = topVec.x + m_Position.x;
		m_Capsule.startPos.y = topVec.y + m_Position.y;
		m_Capsule.startPos.z = topVec.z + m_Position.z;

		vec = { 0.0f,-1.0f,0.0f };

		angle = GetAddAngle(m_Rotation, vec);

		bottomVec.x = { angle.AngleX.x + angle.AngleX.y + angle.AngleX.z };
		bottomVec.y = { angle.AngleY.x + angle.AngleY.y + angle.AngleY.z };
		bottomVec.z = { angle.AngleZ.x + angle.AngleZ.y + angle.AngleZ.z };

		bottomVec = MulFloat(bottomVec, m_CenterPosLngth);

		m_Capsule.endPos.x = bottomVec.x + m_Position.x;
		m_Capsule.endPos.y = bottomVec.y + m_Position.y;
		m_Capsule.endPos.z = bottomVec.z + m_Position.z;


	}

	m_Capsule.radius = m_Size.x;

}


//float CapsuleColiderComponent::SegmentDistance(const XMFLOAT3 vec1start, const XMFLOAT3 vec1end, const XMFLOAT3 vec2start, const XMFLOAT3 vec2end, float& t1, float& t2)
//{
//	XMFLOAT3 d1 = { vec1end.x - vec1start.x, vec1end.y - vec1start.y ,vec1end.z - vec1start.z };
//	XMFLOAT3 d2 = { vec2end.x - vec2start.x, vec2end.y - vec2start.y ,vec2end.z - vec2start.z };
//	XMFLOAT3 r = { vec1start.x - vec2start.x, vec1start.y - vec2start.y ,vec1start.z - vec2start.z };
//
//
//	float a = Dot(d1, d1); //線部1の長さの2乗
//	float e = Dot(d2, d2); //線部2の長さの2乗
//	//float b = Dot(d1, d2);
//	//float c = Dot(d1, r);
//	float f = Dot(d2, r);
//
//	
//	float s = 0.0f, t = 0.0f;
//	//float denom = a * e - b * b;
//	/*if (std::abs(denom) > 1e-6f)
//	{
//		s = (b * f - c * e) / denom;
//		t = (a * f - b * c) / denom;
//	}*/
//
//	if (a > 1e-6f && e > 1e-6)//値が小さすぎなかったら
//	{
//		float b = Dot(d1, d2);
//		float c = Dot(d1, r);
//		float denom = a * e - b * b;
//
//		if (std::abs(denom) > 1e-6f)//線分同士が平行じゃない
//		{
//			
//			s = (b * f - c * e) / denom;
//			t = (a * f - b * c) / denom;
//		}
//
//		s = std::clamp(s, 0.0f, 1.0f);
//		t = std::clamp(t, 0.0f, 1.0f);
//	}
//
//	t1 = s;
//	t2 = t;
//
//	XMFLOAT3 closestPoint1 = { d1.x * s,d1.y * s,d1.z * s};
//	closestPoint1 = { vec1start.x + closestPoint1.x,vec1start.y + closestPoint1.y ,vec1start.z + closestPoint1.z };
//
//	XMFLOAT3 closestPoint2 = { d2.x * t,d2.y * t,d2.z * t };
//	closestPoint2 = { vec2start.x + closestPoint2.x, vec2start.y + closestPoint2.y ,vec2start.z + closestPoint2.z };
//
//	//clossPoint1 = closestPoint1;
//	//clossPoint2 = closestPoint2;
//
//	XMFLOAT3 length = { closestPoint1.x - closestPoint2.x,closestPoint1.y - closestPoint2.y ,closestPoint1.z - closestPoint2.z };
//
//	return LengthSquared(length);
//
//}
//
//bool CapsuleColiderComponent::CapsulesCollision(GameObject* capsuleColider1, GameObject* capsuleColider2)
//{
//	Capsule capsule1 = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule();
//	Capsule capsule2 = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule();
//
//
//	float t1, t2;
//	float distanceSquared = SegmentDistance(capsule1.startPos, capsule1.endPos, capsule2.startPos, capsule2.endPos, t1, t2);
//
//	float radiusSum = capsule1.radius + capsule2.radius;
//
//	if (distanceSquared <= radiusSum * radiusSum)
//	{
//		if (capsuleColider1->GetTag() == "Attack") return true;
//		if (capsuleColider2->GetTag() == "Attack") return true;
//
//		XMFLOAT3 cap1Len = { capsule1.endPos.x - capsule1.startPos.x, capsule1.endPos.y - capsule1.startPos.y ,capsule1.endPos.z - capsule1.startPos.z };
//		XMFLOAT3 cap2Len = { capsule2.endPos.x - capsule2.startPos.x, capsule2.endPos.y - capsule2.startPos.y ,capsule2.endPos.z - capsule2.startPos.z };
//
//		cap1Len = { cap1Len.x * t1,cap1Len.y * t1 ,cap1Len.z * t1 };
//		cap1Len = { cap2Len.x * t2,cap2Len.y * t2 ,cap2Len.z * t2 };
//
//
//		// 衝突時の最近点と法線計算
//		XMFLOAT3 point1 = { capsule1.startPos.x + cap1Len.x,capsule1.startPos.y + cap1Len.y ,capsule1.startPos.z + cap1Len.z };
//		XMFLOAT3 point2 = { capsule2.startPos.x + cap2Len.x,capsule2.startPos.y + cap2Len.y ,capsule2.startPos.z + cap2Len.z };
//
//		XMFLOAT3 pointLen = { point1.x - point2.x,point1.y - point2.y ,point1.z - point2.z };
//
//		XMFLOAT3 normal = Normalize(pointLen);
//
//		//埋まっている深さを計算
//		float penetrationDepth = radiusSum - sqrt(distanceSquared);
//
//		XMFLOAT3 obj1Pos = capsuleColider1->GetComponent<Colider>()->GetPosition();
//		XMFLOAT3 obj2Pos = capsuleColider2->GetComponent<Colider>()->GetPosition();
//
//		XMFLOAT3 vectorObjects = { obj1Pos.x - obj2Pos.x,0.0f ,obj1Pos.z - obj2Pos.z };
//
//		vectorObjects = Normalize(vectorObjects);
//
//		//修正ベクトル
//		XMFLOAT3 correction = {vectorObjects.x * penetrationDepth,vectorObjects.y * penetrationDepth,vectorObjects.z * penetrationDepth};
//		/*XMFLOAT3 correction = MulFloat(normal, penetrationDepth);*/
//		if (Length(correction) <= 0.00001f)
//		{
//			return false;
//		}
//
//
//		//位置を補正
//		//当たったオブジェクトが静的だったら
//		if (capsuleColider2->GetObjectKind() == Static)
//		{
//			XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//			XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//
//			m_Capsule.startPos = Add(myStartPos, correction);
//			m_Capsule.endPos = Add(myEndPos, correction);
//			m_Position = Add(m_Position, correction);
//		
//			m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);
//		}
//		//当たったオブジェクトが動的だったら
//		else if (capsuleColider2->GetObjectKind() == Dynamic)
//		{
//
//			int weight1 = capsuleColider1->GetWeight();
//			int weight2 = capsuleColider2->GetWeight();
//
//
//			//当たったオブジェクトの重さが自分よりも大きかったら
//			if (weight2 > weight1)
//			{
//				XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//				XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//
//				m_Capsule.startPos = Add(myStartPos, correction);
//				m_Capsule.endPos = Add(myEndPos, correction);
//				m_Position = Add(m_Position, correction);
//				
//				m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);
//			}
//			//当たったオブジェクトの重さが自分よりも小さかったら
//			else if (weight2 < weight1)
//			{
//				XMFLOAT3 otherPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetPosition();
//				XMFLOAT3 otherStartPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//				XMFLOAT3 otherEndPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//				CapsuleColiderComponent* col = capsuleColider2->GetComponent<CapsuleColiderComponent>();
//
//
//				col->SetCapsuleStartPos(Sub(otherStartPos, correction));
//				col->SetCapsuleStartPos(Sub(otherEndPos, correction));
//
//				XMFLOAT3 pos = Sub(otherPos, correction);
//				col->SetPosition(pos);
//				capsuleColider2->GetComponent<Transform>()->SetPosition(pos);
//			}
//			//当たったオブジェクトの重さが自分と同じだったら
//			else if (weight2 == weight1)
//			{
//				//自分を動かす
//				XMFLOAT3 myStartPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//				XMFLOAT3 myEndPos = capsuleColider1->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//
//				correction = MulFloat(correction, 0.5f);
//
//				m_Capsule.startPos = Add(myStartPos, correction);
//				m_Capsule.endPos = Add(myEndPos, correction);
//				m_Position = Add(m_Position, correction);
//
//				m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);
//				//相手を動かす
//				XMFLOAT3 otherPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetPosition();
//				XMFLOAT3 otherStartPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().startPos;
//				XMFLOAT3 otherEndPos = capsuleColider2->GetComponent<CapsuleColiderComponent>()->GetCapsule().endPos;
//				CapsuleColiderComponent* col = capsuleColider2->GetComponent<CapsuleColiderComponent>();
//
//
//				col->SetCapsuleStartPos(Sub(otherStartPos, correction));
//				col->SetCapsuleStartPos(Sub(otherEndPos, correction));
//
//				XMFLOAT3 pos = Sub(otherPos, correction);
//				col->SetPosition(pos);
//				capsuleColider2->GetComponent<Transform>()->SetPosition(pos);
//			}
//		}
//
//
//		return true;
//
//	}
//
//	return false;
//
//}
//
////OBBの当たり判定
//bool CapsuleColiderComponent::CapsuleBoxCollision(Capsule& capsule, GameObject* obj, XMFLOAT3& moveDir)
//{
//
//	const float MIN = 0.00001f; //無視する値
//	const int MAX_COUNT = 5;//最大処理回数
//
//	Box box = obj->GetComponent<BoxColiderComponent>()->GetOBB();
//	
//	Angle angle;
//	
//	int count = 0;
//	while (count < MAX_COUNT)
//	{
//		// カプセルのローカル座標変換
//		XMFLOAT3 localStart = ToLocalSpace(capsule.startPos, box);
//		XMFLOAT3 localEnd = ToLocalSpace(capsule.endPos, box);
//
//		// ボックスの範囲内にクランプ
//		XMFLOAT3 clampedStart = clampToBox(localStart, box.halfSize);
//		XMFLOAT3 clampedEnd = clampToBox(localEnd, box.halfSize);
//
//		// 衝突判定（始点または終点が埋まっている場合）
//		float distStartSquared = Dot(Sub(localStart, clampedStart), Sub(localStart, clampedStart));
//		float distEndSquared = Dot(Sub(localEnd, clampedEnd), Sub(localEnd, clampedEnd));
//
//		//衝突判定
//		if (distStartSquared > capsule.radius * capsule.radius && distEndSquared > capsule.radius * capsule.radius) {
//			return count > 0; // 一度でも当たったらtrueを返す
//		}
//
//		//一番深いところのポジション
//		XMFLOAT3 DeepPoint;
//
//		//埋まった深さの線分の始点と終点
//		XMFLOAT3 penetrationStart;
//		XMFLOAT3 penetrationEnd;
//
//
//		if (distStartSquared > distEndSquared)
//		{
//			DeepPoint = clampedEnd;// 最も深く埋まったボックス側の点
//			penetrationStart = localEnd;// カプセル側の点（終点）
//			penetrationEnd = clampedEnd;// ボックス側の点
//		}
//		else
//		{
//			DeepPoint = clampedStart;// 最も深く埋まったボックス側の点
//			penetrationStart = localStart;// カプセル側の点（始点）
//			penetrationEnd = clampedStart;// ボックス側の点
//		}
//
//		XMFLOAT3 penetrationVector = Sub(penetrationStart, penetrationEnd);
//
//
//		// 埋まり深さを計算
//		float penetrationDepth = capsule.radius - Length(penetrationVector);
//		if (penetrationDepth <= MIN) {
//			break; // 埋まりがない場合処理を終了
//		}
//
//		//カプセルの法線
//		XMFLOAT3 localNormal = Normalize(penetrationVector);
//		//当たったボックスの面の法線
//		XMFLOAT3 worldNormal = Sub(ToWorldSpace(localNormal, box), box.center);
//
//		XMFLOAT3 correction = MulFloat(Normalize(worldNormal), penetrationDepth);
//
//		DeepPoint = ToWorldSpace(DeepPoint, box);
//		penetrationStart = ToWorldSpace(penetrationStart, box);
//		penetrationEnd = ToWorldSpace(penetrationEnd, box);
//
//		float PI = 3.14159265359 / 2;
//		/*(PI / 180.0f)* angle.x;*/
//		XMFLOAT3 t = { 0.0f,1.0f,0.0f };
//
//		float DotProdct = Dot(t, worldNormal);
//		float len = Length(t) * Length(worldNormal);
//
//		float cosineTheta = DotProdct / len;
//
//		cosineTheta = std::fmax(-1.0f, std::fmin(1.0f, cosineTheta));
//
//		//二つのベクトルのなす角
//		float radian = std::acos(cosineTheta);
//
//		XMVECTOR deep = { DeepPoint.x,DeepPoint.y,DeepPoint.z,0.0f };
//		XMVECTOR cE = { clampedEnd.x,clampedEnd.y ,clampedEnd.z ,0.0f };
//
//		//法線と真上のベクトルのなす角が50度より小さかったら
//		if (radian <= 50.0f * (3.14159265359 / 180.0f) && penetrationStart.y <= capsule.endPos.y + m_Size.x)
//		{
//
//			float a = 90.0f;
//			float b = 90.0f - (radian / ((2 * PI) / 180.0f));
//			float c = 180.0f - (a + b);
//
//			//半径
//			float r = (penetrationDepth / sinf(a)) / 2;
//
//			float A = (2 * r) * sinf(a);//aの角の対の辺
//			float B = (2 * r) * sinf(b);//bの角の対の辺
//			float C = (2 * r) * sinf(c);//今わかっている長さ
//
//			float baiA = sqrt((B * B) + (C * C));
//
//			float ratioA = A / C;
//
//			float reA = ratioA * penetrationDepth;
//
//			//進んでる方向の逆ベクトル
//			XMFLOAT3 reverseVelocity = { 0.0f,worldNormal.y,0.0f };
//			XMFLOAT3 reverseVector = Normalize(reverseVelocity);
//
//			reverseVector = { reverseVector.x * baiA,reverseVector.y * baiA,reverseVector.z * baiA };
//
//			/*m_Position = Add(m_Position, reverseVector);*/
//			//埋まりを解消
//			//当たったオブジェクトが静的だったら
//			if (obj->GetObjectKind() == Static)
//			{
//				capsule.startPos = Add(capsule.startPos, reverseVector);
//				capsule.endPos = Add(capsule.endPos, reverseVector);
//				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
//				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
//			}
//			//当たったオブジェクトが動的だったら
//			else if (obj->GetObjectKind() == Dynamic)
//			{
//				capsule.startPos = Add(capsule.startPos, reverseVector);
//				capsule.endPos = Add(capsule.endPos, reverseVector);
//				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
//				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
//			}
//
//			//オブジェクトに乗っているオブジェクトの重力をoff
//			m_GameObject->SetGravityState(false);
//			m_GameObject->SetGravitiScale(0.05f);
//		
//		}
//		else
//		{
//
//			float a = 90.0f;
//			float b = 90.0f - (radian / ((2 * PI) / 180.0f));
//			float c = 180.0f - (a + b);
//
//			//半径
//			float r = (penetrationDepth / sinf(a)) / 2;
//
//			float A = (2 * r) * sinf(a);//aの角の対の辺
//			float B = (2 * r) * sinf(b);//bの角の対の辺
//			float C = (2 * r) * sinf(c);//今わかっている長さ
//
//			float baiA = sqrt((B * B) + (C * C));
//
//			//法線ベクトル
//			XMFLOAT3 reverseVelocity = { worldNormal.x,0.0f,worldNormal.z };
//			XMFLOAT3 reverseVector = Normalize(reverseVelocity);
//
//			reverseVector = { reverseVector.x * baiA,reverseVector.y * baiA,reverseVector.z * baiA };
//
//			/*m_Position = Add(m_Position, reverseVector);*/
//			//当たったオブジェクトが静的だったら
//			if (obj->GetObjectKind() == Static)
//			{
//				capsule.startPos = Add(capsule.startPos, reverseVector);
//				capsule.endPos = Add(capsule.endPos, reverseVector);
//				XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
//				m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, reverseVector));
//			}
//			//当たったオブジェクトが動的だったら
//			else if (obj->GetObjectKind() == Dynamic)
//			{
//				//当たったオブジェクトの重さが自分よりも大きかったら
//				if (obj->GetWeight() > m_GameObject->GetWeight())
//				{
//					capsule.startPos = Add(capsule.startPos, reverseVector);
//					capsule.endPos = Add(capsule.endPos, reverseVector);
//					XMFLOAT3 Pos = obj->GetComponent<Colider>()->GetPosition();
//					Pos = Add(Pos, reverseVector);
//
//					obj->GetComponent<Colider>()->SetPosition(Pos);
//					obj->GetComponent<Transform>()->SetPosition(Pos);
//				}
//				//当たったオブジェクトの重さが自分よりも小さかったら
//				else if (obj->GetWeight() < m_GameObject->GetWeight())
//				{
//					XMFLOAT3 Pos = obj->GetComponent<Colider>()->GetPosition();
//					Pos = Sub(Pos, reverseVector);
//
//					obj->GetComponent<Colider>()->SetPosition(Pos);
//					obj->GetComponent<Transform>()->SetPosition(Pos);
//				}
//				//当たったオブジェクトの重さが自分と同じだったら
//				else if (obj->GetWeight() == m_GameObject->GetWeight())
//				{
//					//自分を動かす
//					capsule.startPos = Add(capsule.startPos, MulFloat(reverseVector,0.5f));
//					capsule.endPos = Add(capsule.endPos, MulFloat(reverseVector, 0.5f));
//					XMFLOAT3 transPos = m_GameObject->GetComponent<Transform>()->GetPosition();
//					m_GameObject->GetComponent<Transform>()->SetPosition(Add(transPos, MulFloat(reverseVector, 0.5f)));
//
//					//相手を動かす
//					XMFLOAT3 Pos = obj->GetComponent<Colider>()->GetPosition();
//					Pos = Sub(Pos, MulFloat(reverseVector,0.5f));
//
//					obj->GetComponent<Colider>()->SetPosition(Pos);
//					obj->GetComponent<Transform>()->SetPosition(Pos);
//				}
//			}
//
//		}
//		count++;
//		m_Correction = correction;
//	}
//	/*m_Position = Add(m_Position, worldCorrection);*/
//
//	//// 法線に基づいて移動ベクトルを修正（壁ずりを考慮
//	//XMFLOAT3 normal = Normalize(correction, NULL);
//
//
//	//float dotProduct = Dot(moveDir, worldNormal);
//	//XMFLOAT3 velo = Sub(moveDir, MulFloat(worldNormal, dotProduct));// 法線方向を除去して面沿いの動きにする
//
//}


//あった判定と最初に当たったオブジェクトと当たっているオブジェクトリストを返します
std::tuple<bool, GameObject*, std::list<GameObject*>> CapsuleColiderComponent::GetCollision()
{
	int objSize = 0;
	std::list<GameObject*> objList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

	//ボックスにあった時の法線の情報
	std::vector<std::pair<XMFLOAT3, XMFLOAT3>> CollsionDate;

	//自分以外のコライダーのポジションとサイズ
	XMFLOAT3 pos;
	XMFLOAT3 size;

	for (auto obj : Scene::GetInstance()->GetScene<GameScene>()->GetGameObjectList(1))
	{
		if (obj->GetComponent<Colider>() == nullptr) {
			continue;
		}
		if (GetGameObject() == obj) {
			continue;
		}

		pos = obj->GetComponent<Colider>()->GetPosition();
		size = obj->GetComponent<Colider>()->GetSize();

		//コライダーがカプセルだったら
		if (obj->GetComponent<Colider>()->GetColiderKind() == CAPSULE)
		{

			if (CapsulesCollision(this->m_GameObject, obj))
			{

				objList.push_back(obj);
				objSize = objList.size();

			}
		}
		//コライダーがBOXだったら
		if (obj->GetComponent<Colider>()->GetColiderKind() == BOX)
		{
			XMFLOAT3 collisionNormal;
			Box otherObb = obj->GetComponent<BoxColiderComponent>()->GetOBB();


			if (CapsuleBoxCollision(m_GameObject, obj))
			{
				objList.push_back(obj);
				objSize = objList.size();
			}

		}

	}
	
	if (objSize != 0) 
	{
		auto itr =objList.begin();
		GameObject* gameObject = (*itr);
	
		std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject = std::make_tuple(true,gameObject,objList);
		return OnCollisionObject;
	}
	else if (objSize == 0) 
	{

		std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject = std::make_tuple(false, nullptr, objList);
		return OnCollisionObject;

	}

}




