#include "main.h"
#include "cylinderColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "transform3DComponent.h"


void CylinderColiderComponent::Init()
{
	m_Position = GetGameObject()->GetComponent<Transform3DComponent>()->GetPosition();
	m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();
	m_Rotation = GetGameObject()->GetComponent<Transform3DComponent>()->GetRotation();
}

void CylinderColiderComponent::Uninit()
{
}

void CylinderColiderComponent::Update()
{
	m_Position = GetGameObject()->GetComponent<Transform3DComponent>()->GetPosition();
	m_Size = GetGameObject()->GetComponent<Transform3DComponent>()->GetSize();
	m_Rotation = GetGameObject()->GetComponent<Transform3DComponent>()->GetRotation();
}

void CylinderColiderComponent::Draw()
{
}

bool CylinderColiderComponent::IsCollision()
{
	XMFLOAT3 parentTransPos = GetGameObject()->GetComponent<Transform3DComponent>()->GetPosition();
	XMFLOAT3 parentColiderPos = GetGameObject()->GetComponent<Colider>()->GetPosition();
	XMFLOAT3 parentColiderSize = GetGameObject()->GetComponent<Colider>()->GetSize();
	XMFLOAT3 pos;
	XMFLOAT3 size;

	std::list<GameObject*> objlist = Scene::GetInstance()->GetScene<GameScene>()->GetGameObjectList(1);

	for (auto obj : objlist)
	{

		if (obj->GetComponent<Colider>() == nullptr) {
			continue;
		}
		if (GetGameObject() == obj) {
			continue;
		}

		pos = obj->GetComponent<CylinderColiderComponent>()->GetPosition();
		size = obj->GetComponent<CylinderColiderComponent>()->GetSize();

		if (parentColiderPos.z - (parentColiderSize.z) <= pos.z + (size.z) &&
			parentColiderPos.z + (parentColiderSize.z) >= pos.z - (size.z) &&
			parentColiderPos.x - (parentColiderSize.x) <= pos.x + (size.x) &&
			parentColiderPos.x + (parentColiderSize.x) >= pos.x - (size.x) &&
			parentColiderPos.y - (parentColiderSize.y / 2) <= pos.y + (size.y) &&
			parentColiderPos.y + (parentColiderSize.y) >= pos.y - (size.y)
			)
		{

			return true;
		}
	}

	return false;
}

//あった判定と最初に当たったオブジェクトと当たっているオブジェクトリストを返します
std::tuple<bool, GameObject*, std::list<GameObject*>> CylinderColiderComponent::GetCollision()
{
	int objSize = 0;
	std::list<GameObject*> objList;
	std::tuple<bool, GameObject*, std::list<GameObject*>> OnCollisionObject;

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

		//お互いの距離
		XMFLOAT3 direction;
		direction.x = pos.x - m_Position.x;
		direction.y = pos.y - m_Position.y;
		direction.z = pos.z - m_Position.z;

		float length;
		length = sqrtf(direction.x * direction.x + direction.z * direction.z);


		if (length < size.x)
		{

			objList.push_back(obj);
			objSize = objList.size();
			if (-direction.y > size.y - 0.5f) {
				
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
