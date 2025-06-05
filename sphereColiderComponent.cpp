#include "main.h"
#include "sphereColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "boxColiderComponent.h"
#include "titleScene.h"

void SphereColiderComponent::Init()
{
	m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
	m_Size = GetGameObject()->GetComponent<Transform>()->GetSize();
	m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();

	m_ColiderKind = SPHERE;

	m_Sphere =
	{
		m_Position,
		m_Size.x
	};

}

void SphereColiderComponent::Uninit()
{
}

void SphereColiderComponent::Update()
{
	m_Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
	/*m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();*/
	m_Rotation = GetGameObject()->GetComponent<Transform>()->GetRotation();
	m_Velocity = GetGameObject()->GetComponent<Transform>()->GetVelocity();

	//コライダーのポジション調整のため
	m_Position = Add(m_Position, m_AddPosition);

	m_Sphere =
	{
		m_Position,
		m_Size.x
	};

	GetCollision();
}

void SphereColiderComponent::Draw()
{
}

bool SphereColiderComponent::SphereOBBCollision(const Box box, Sphere sphere, XMFLOAT3& collisionpoint)
{

	const int MaxCount = 10;
	int count = 0;
	bool collisionResolved = false;
	bool DoOnce = false;

	while (count < MaxCount)
	{

		XMFLOAT3 localSpherePos = ToLocalSpace(sphere.center, box);

		// ローカル空間での最近点を計算（AABBにクランプ）
		XMFLOAT3 closestPoint =
		{
			std::clamp(localSpherePos.x,-box.halfSize.x,box.halfSize.x),
			std::clamp(localSpherePos.y,-box.halfSize.y,box.halfSize.y),
			std::clamp(localSpherePos.z,-box.halfSize.z,box.halfSize.z)

		};

		// 最近点をワールド空間に変換
		collisionpoint = ToWorldSpace(closestPoint, box);

		// 球体の中心と最近点の距離を計算
		XMFLOAT3 length = Sub(localSpherePos, closestPoint);
		float distanceSquared = LengthSquared(length);

		// 衝突判定：距離の平方が球体の半径の平方以下であれば衝突
		if (distanceSquared > sphere.radius * sphere.radius)
		{
			return count > 0;
		}

		XMFLOAT3 penetrationDepths;

		// 球体がボックス内部に完全に埋まっている場合の法線計算
		XMFLOAT3 localNormal;
		if (distanceSquared < 1e-4f) {
			penetrationDepths = {
				box.halfSize.x - std::abs(localSpherePos.x),
				box.halfSize.y - std::abs(localSpherePos.y),
				box.halfSize.z - std::abs(localSpherePos.z)
			};

			if (penetrationDepths.x <= penetrationDepths.y && penetrationDepths.x <= penetrationDepths.z) {
				localNormal = { localSpherePos.x < 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f };
			}
			else if (penetrationDepths.y <= penetrationDepths.z) {
				localNormal = { 0.0f, localSpherePos.y < 0.0f ? -1.0f : 1.0f, 0.0f };
			}
			else {
				localNormal = { 0.0f, 0.0f, localSpherePos.z < 0.0f ? -1.0f : 1.0f };
			}
		}
		else {
			localNormal = Normalize(length);
		}

		XMFLOAT3 worldNormal = Sub(ToWorldSpace(localNormal, box), box.center);
		worldNormal = Normalize(worldNormal);


		//埋まっている深さの始点と終点
		XMFLOAT3 penetrationStart = ToWorldSpace(closestPoint, box);
		//XMFLOAT3 penetrationEnd = Add(penetrationStart,MulFloat(worldNormal,sphere.radius));

		XMFLOAT3 penetrationEnd = sphere.center;

		float penetrationDepth = sphere.radius - sqrt(distanceSquared);

		//反射させる
		if (DoOnce == false)
		{
			ReflectSphere(sphere, localNormal);
			DoOnce = true;
		}

		if (penetrationDepth < 1e-4f)
		{
			return count > 0;
		}

		/*XMFLOAT3 penetrationVector = { normal.x * penetrationDepth ,normal.y * penetrationDepth ,normal.z * penetrationDepth };

		penetrationVector = MulFloat(penetrationVector, 1.1f);*/

		m_Position = Add(m_Position,MulFloat(worldNormal,penetrationDepth));
		/*m_GameObject->GetComponent<Transform>()->SetPosition(m_Position);*/
		sphere.center = m_Position;
		collisionResolved = true;

		count++;
	}

	return collisionResolved;
}

void SphereColiderComponent::ReflectSphere(Sphere sphere, const XMFLOAT3 normal)
{

	XMFLOAT3 a = Mul(m_Velocity, normal);

	//反射ベクトルの公式 反射ベクトル = 現在の速度 - 2 * (現在の速度 * 法線) * 法線
	m_Velocity = Sub(m_Velocity,MulFloat(Mul(a,normal),2.0f));
	m_GameObject->GetComponent<Transform>()->SetVelocity(m_Velocity);

}

//あった判定と最初に当たったオブジェクトと当たっているオブジェクトリストを返します
std::tuple<bool, GameObject*, std::list<GameObject*>> SphereColiderComponent::GetCollision()
{
	int objSize = 0;
	std::list<GameObject*> objList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

	//自分以外のコライダーのポジションとサイズ
	XMFLOAT3 pos;
	XMFLOAT3 size;

	std::list<GameObject*> list;
	if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == TITLE)
	{
		list = Scene::GetInstance()->GetScene<TitleScene>()->GetGameObjectList(1);
	}
	else if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == GAME)
	{
		list = Scene::GetInstance()->GetScene<GameScene>()->GetGameObjectList(1);
	}


	for (auto obj : list)
	{
		if (obj->GetComponent<Colider>() == nullptr) {
			continue;
		}
		if (GetGameObject() == obj) {
			continue;
		}

		pos = obj->GetComponent<Colider>()->GetPosition();
		size = obj->GetComponent<Colider>()->GetSize();
		
		if (obj->GetComponent<Colider>()->GetColiderKind() == BOX)
		{
			Box obb = obj->GetComponent<BoxColiderComponent>()->GetOBB();

			XMFLOAT3 point;

			if (SphereOBBCollision(obb, m_Sphere, point))
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

