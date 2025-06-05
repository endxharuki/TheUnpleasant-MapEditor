#include "main.h"
#include "player.h"
#include "transform3DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "explosion.h"
#include "transform2DComponent.h"
#include "boxColiderComponent.h"
#include "enemy.h"

void Bullet::Init()
{

	AddComponent<Transform3DComponent>();
	GetComponent<Transform3DComponent>()->AddModelData("asset\\Model\\bullet.obj");
	AddComponent<BoxColiderComponent>()->SetTransform(
	GetComponent<Transform3DComponent>()->GetPosition(),
	GetComponent<Transform3DComponent>()->GetRotation(),
	GetComponent<Transform3DComponent>()->GetSize());
	frameCount = 0.0f;

	GetComponent<Colider>()->SetSize(XMFLOAT3(1.0f, 1.0f, 1.0f));
	Tag = "Bullet";

}

void Bullet::Uninit()
{

	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void Bullet::Update()
{
	
	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPosition();
	XMFLOAT3 rot = GetComponent<Transform3DComponent>()->GetRotation();
	GetComponent<Transform3DComponent>()->SetOldPosition(pos);
	GetComponent<BoxColiderComponent>()->SetOldPosition(pos);
	float x = sinf(rot.y);
	float z = cosf(rot.y);
	GetComponent<Transform3DComponent>()->SetSize(XMFLOAT3(5.0f, 5.0f, 5.0f));
	GetComponent<Colider>()->SetSize(XMFLOAT3(1.0f, 1.0f, 1.0f));
    pos = XMFLOAT3(pos.x += x, pos.y, pos.z += z);

	for (auto com :ComponentList)
	{
		com->Update();
	}
	if (frameCount >= 10.0f) {
		isDelete = true;
		Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<Explosion>(2)->GetComponent<Transform2DComponent>()->SetPosition(XMFLOAT3(pos.x, pos.y, pos.z));

	}

	//セットしている
	GetComponent<Transform3DComponent>()->SetPosition(pos);
	
	//当たり判定を取得
	if (std::get<0>(GetComponent<BoxColiderComponent>()->GetCollision()) == true) {
		//当たっている全てのオブジェクトのリストを取得
		std::list<GameObject*> objs = std::get<2>(GetComponent<Colider>()->GetCollision());
		for (auto onCollisionObj : objs)
		{
			if (onCollisionObj->GetTag() == "Enemy") {

				//当たったオブジェクトを消す
				onCollisionObj->SetDelete(true);
				//自分も消す
				isDelete = true;
				break;
			}
		}
	}

	frameCount++;
}

void Bullet::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
