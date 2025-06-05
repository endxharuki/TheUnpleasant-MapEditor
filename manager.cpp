#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "gameScene.h"
#include "titleScene.h"
#include "mapCreateScene.h"
#include "resultScene.h"
#include "texturePool.h"
#include "texturePool.h"



void Manager::Init()
{

	/*TexturePool::GetInstance()->LoadTextrue();*/
	Scene::GetInstance()->InitSceneState(new MapCreateScene);

}


void Manager::Uninit()
{
	
	Scene::GetInstance()->Uninit();
	//Renderer::Uninit();
}

void Manager::Update()
{

	
	Scene::GetInstance()->Update();

}

void Manager::Draw()
{
	
	Scene::GetInstance()->Draw();
}
