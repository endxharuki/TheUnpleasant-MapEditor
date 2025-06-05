#include "main.h"
#include "gameSky.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"


void GameSky::Init()
{
	AddComponent<Transform3DComponent>()->AddModelData("asset\\Model\\sky1.obj");
	
	GetComponent<Transform3DComponent>()->SetSize(XMFLOAT3(600.0f, 600.0f, 600.0f));
	GetComponent<Transform>()->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	
	Tag = "Sky";

}

void GameSky::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void GameSky::Update()
{

	XMFLOAT3 TransformPos = GetComponent<Transform3DComponent>()->GetPosition();
	

	for (auto com : ComponentList)
	{
		com->Update();

	}
}

void GameSky::Draw()
{
	
	for (auto com : ComponentList)
	{
		com->Draw();
	}
}

