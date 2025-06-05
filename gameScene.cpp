#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "playerCamera.h"
#include "field.h"
#include "player.h"
#include "gameScene.h"
#include "resultScene.h"
#include "scene.h"
#include "enemy.h"
#include "block.h"
#include "explosion.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "score.h"
#include "timeLimit.h"
#include "sky.h"
#include "goal.h"
#include "resultScene.h"
#include "meshField.h"
#include "house.h"
#include "treeFactory.h"
#include "tree.h"
#include "fade.h"
#include "texturePool.h"
#include "input.h"
#include "shadowVolume.h"
#include "gameSky.h"
#include "magicBlade.h"
#include "textHp.h"
#include "textGauge.h"
//#include "slashFirst.h"


void GameScene::Init()
{

	m_SceneName = GAME;

	//レイヤー順番バカ大事だから気を付けて
	TexturePool::GetInstance()->LoadTextrue();
	TexturePool::GetInstance()->Init();
	Input::GetInstance()->Init();

	AddGameObject<PlayerCamera>(0);//2Dのゲームオブジェクトを入れたらカメラは一番最後


	AddGameObject<Player>(1)->GetComponent<Colider>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));


	AddGameObject<GameSky>(1);
	AddGameObject<MeshField>(1);

	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(5.0f, 3.0f, 20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));
	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(20.0f, 0.0f, -39.0f), XMFLOAT3(45.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));
	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(25.0f, 3.0f, -30.0f), XMFLOAT3(0.0f, 45.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));
	AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(10.0f, 0.0f, -25.0f), XMFLOAT3(44.0f, 0.0f, 40.0f), XMFLOAT3(3.0f, 3.0f, 3.0f));



	AddGameObject<Enemy>(1)->GetComponent<Transform>()->SetTransformRotAngle(XMFLOAT3(5.0f, 0.0f, -110.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.02f, 0.02f, 0.02f));




	//シャドウボリューム
	AddGameObject<ShadowVolume>(1);
	AddGameObject<Polygon2D>(1);

	


	AddUIObject<Fade>()->SetPosTexNumber(0.0f, 0.0f, 1400.0f, 800.0f, TexturePool::GetInstance()->GetTexture("Fade"), 1);
	GetUIObject<Fade>(1)->SetOnDraw(true);
	GetUIObject<Fade>(1)->SetFadeState(In);

	AddUIObject<TextHp>();
	AddUIObject<TextGauge>();

	for (auto tex : m_TextureList) {
		tex->Init();
	}
	
	imgui = new ImguiManager;
	imgui->Init();
}

void GameScene::Uninit()
{
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Uninit();
			delete obj;
		}
		if (i == 1)
		{
			for (auto tex : m_TextureList)
			{
				tex->Uninit();
			}
		}
		m_GameObjectList[i].clear();
	}
	TexturePool::GetInstance()->UnLoadTexture();
	imgui->Uninit();
	Input::GetInstance()->Uninit();
	
	Renderer::Uninit();
}

void GameScene::Update()
{
	Input::GetInstance()->Update();

	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Update();
		}
		if (i == 1)
		{
			for (auto tex : m_TextureList)
			{
				tex->Update();
			}
		}
	}

	DeleteGameObject();

	
	if (isGoal == true)
	{
		AddGameObject<Goal>(1)->GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 220.0f));
	}
	if (isClear == true)
	{
		Uninit();
		Scene::GetInstance()->ChangeSceneState(new ResultScene);
	}
	/*if (GetGameObject<Player>()->GetScore() >= 3) 
	{
		Uninit();
		Scene::GetInstance()->ChangeSceneState(new ResultScene);
	}*/
}

void GameScene::Draw()
{

	Renderer::Begin();
	
	imgui->ImGuiRendererInit();

	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i])
		{
			obj->Draw();
		}
		if (i == 1)
		{
			for (auto tex : m_TextureList)
			{
				tex->Draw();
			}
		}
	}
	imgui->ImGuiRenderer();
	
	Renderer::End();
}

void GameScene::DeleteGameObject()
{
	for (int i = 0;i < 3;i++) {
		m_GameObjectList[i].remove_if([](GameObject* obj) {

			if (obj->DeleteRecest() == true) {
			  //if(obj->GetTag() == "Enemy") Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->SetScore(1);
				obj->Uninit();
				delete obj;
				return true;
			}

			return false;
			}
		);
	}
	
	m_TextureList.remove_if([](UI* obj) {

		if (obj->DeleteRecest() == true)
		{
			obj->Uninit();
			delete obj;

			return true;
		}
		return false;
		}
	);
}
