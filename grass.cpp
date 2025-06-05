#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "boxColiderComponent.h"
#include "capsuleColiderComponent.h"
#include "grass.h"
#include "meshField.h"
#include "texturePool.h"

void Grass::Init()
{

	AddComponent<Transform3DComponent>()->AddModelData("asset\\Model\\grass\\allGrass_001.obj");
	GetComponent<Transform>()->SetSize(XMFLOAT3(2.0f,2.0f,2.0f));
	AddComponent<CapsuleColiderComponent>()->SetSize(XMFLOAT3(0.5f, 0.5f, 0.5f));
	GetComponent<CapsuleColiderComponent>()->SetCenterPosLnegth(1.0f);

	Tag = "Tree";

}

void Grass::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void Grass::Update()
{

	XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
	float hight = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<MeshField>()->GetHeight(pos);
	float coliderPosY = GetComponent<Colider>()->GetSize().y;

	GetComponent<Transform>()->SetPosition(XMFLOAT3(pos.x, hight + coliderPosY, pos.z));

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void Grass::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
