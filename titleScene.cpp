#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "titleScene.h"
#include "gameScene.h"
#include "scene.h"
#include "enemy.h"
#include "block.h"
#include "explosion.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "fade.h"
#include "texturePool.h"
#include "field.h"
#include "input.h"
#include "titleCamera.h"
#include "wave.h"
#include "meshField.h"
#include "sky.h"
#include "star.h"
#include "titleMeshField.h"
#include "titleBoard.h"
#include "titleHideBoard.h"
#include "pushToSpaceBoard.h"

void TitleScene::Init()
{

	m_SceneName = TITLE;

	audio = new Audio;

	audio->InitMaster();
	audio->Load("asset\\Audio\\titleBGM.wav");
	//タイトルでしかレンダラーの初期化しなくてええよ
	Renderer::Init();
	

	Input::GetInstance()->Init();

	TexturePool::GetInstance()->LoadTextrue();

	audio->Play(false);
	audio->SetVolume(0.9f);
	//座標とテクスチャとナンバーを設定
	/*AddUIObject<Texture2D>()->SetPosTexNumber(0.0f, 0.0f, 1300.0f, 730.0f, TexturePool::GetInstance()->GetTexture("TitleBack"), 1);
	AddUIObject<Texture2D>()->SetPosTexNumber(420.0f, 500.0f, 500.0f, 180.0f, TexturePool::GetInstance()->GetTexture("PushSpace"), 2);
	AddUIObject<Texture2D>()->SetPosTexNumber(300.0f, 200.0f, 800.0f, 200.0f, TexturePool::GetInstance()->GetTexture("TitleName"), 3);*/
	/*AddGameObject<Field>(1);*/
	/*AddGameObject<MeshField>(1);*/
	
	AddGameObject<Sky>(1)->GetComponent<Transform>()->SetSize(XMFLOAT3(300.0f,300.0f,300.0f));
	AddGameObject<TitleMeshField>(1);
	AddGameObject<Wave>(1);

	//横の壁
	AddGameObject<Block>(1)->GetComponent<Transform3DComponent>()->SetTransformRotAngle(XMFLOAT3(0.0f, 50.0f, -200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(110.0f, 100.0f, 110.0f));
	AddGameObject<Block>(1)->GetComponent<Transform3DComponent>()->SetTransformRotAngle(XMFLOAT3(0.0f, 50.0f, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(110.0f, 100.0f, 110.0f));
	AddGameObject<Block>(1)->GetComponent<Transform3DComponent>()->SetTransformRotAngle(XMFLOAT3(200.0f, 50.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(110.0f, 100.0f, 110.0f));
	AddGameObject<Block>(1)->GetComponent<Transform3DComponent>()->SetTransformRotAngle(XMFLOAT3(-200.0f, 50.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(110.0f, 100.0f, 110.0f));
	//下の床
	AddGameObject<Block>(1)->GetComponent<Transform3DComponent>()->SetTransformRotAngle(XMFLOAT3(0.0f, -101.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(110.0f, 100.0f, 110.0f));
	//上の天井
	AddGameObject<Block>(1)->GetComponent<Transform3DComponent>()->SetTransformRotAngle(XMFLOAT3(0.0f, 220.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(110.0f, 100.0f, 110.0f));

	//スター
	AddGameObject<Star>(1)->GetComponent<Transform>()->SetVelocity(XMFLOAT3(0.0f,0.5f,0.2f));
	AddGameObject<Star>(1)->GetComponent<Transform>()->SetVelocity(XMFLOAT3(0.6f, 0.1f, 0.2f));
	AddGameObject<Star>(1)->GetComponent<Transform>()->SetVelocity(XMFLOAT3(0.0f, 0.01f, 1.0f));
	AddGameObject<Star>(1)->GetComponent<Transform>()->SetVelocity(XMFLOAT3(0.5f, 0.5f, 0.5f));
	AddGameObject<Star>(1)->GetComponent<Transform>()->SetVelocity(XMFLOAT3(0.1f, 0.0f, 0.1f));
	AddGameObject<Star>(1)->GetComponent<Transform>()->SetVelocity(XMFLOAT3(0.4f, 0.1f, 0.2f));
	AddGameObject<Star>(1)->GetComponent<Transform>()->SetVelocity(XMFLOAT3(0.2f, 1.0f, 0.0f));
	AddGameObject<TitleBoard>(1)->GetComponent<Transform>()->SetPosition(XMFLOAT3(0.0f, 30.0f, 100.0f));
	

	//AddGameObject<TitleHideBoard>(1)->GetComponent<Transform>()->SetPosition(XMFLOAT3(0.0f, 50.0f, 10.0f));


	AddUIObject<Fade>()->SetPosTexNumber(0.0f, 0.0f, 1400.0f, 800.0f, TexturePool::GetInstance()->GetTexture("Fade"), 1);
	GetUIObject<Fade>(1)->SetOnDraw(true);
	GetUIObject<Fade>(1)->SetFadeState(In);
	AddUIObject<Fade>()->SetPosTexNumber(0.0f, 0.0f, 1400.0f, 800.0f, TexturePool::GetInstance()->GetTexture("Fade"), 2);
	GetUIObject<Fade>(2)->SetOnDraw(false);
	AddGameObject<TitleCamera>(2);



	for (auto tex : m_TextureList)
	{

		tex->Init();
	}
	//レイヤー順番バカ大事だから気を付けて
	/*audio->Play(false);*/

}

void TitleScene::Uninit()
{
	for (int i = 0; i < 3; i++) {
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

	audio->Uninit();
	delete audio;

	Input::GetInstance()->Uninit();

}

void TitleScene::Update()
{
	Input::GetInstance()->Update();

	//シーンを切り替える前にDeletegameObjectを呼ばないとエラー出る
	if (Input::GetInstance()->GetKeyTrigger(VK_SPACE)) {

		GetUIObject<Fade>(2)->SetFadeState(Out);
		GetUIObject<Fade>(2)->SetOnDraw(true);

	}

	if (Input::GetInstance()->GetKeyTrigger(VK_RETURN))
	{
		float x = ((rand() % 10) / 5.0f) - 1.0f;
		float y = ((rand() % 10) / 5.0f) - 1.0f;
		float z = ((rand() % 10) / 5.0f) - 1.0f;
		AddGameObject<Star>(1)->GetComponent<Transform>()->SetVelocity(XMFLOAT3(x, y, z));
	}
	


	for (int i = 0; i < 3; i++) {
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

	/*drawFrame++;
	if (drawFrame >= 40.0f)
	{

		GetUIObject<Texture2D>(2)->SetOnDraw(false);
	}
	if (drawFrame >= 80.0f)
	{

		GetUIObject<Texture2D>(2)->SetOnDraw(true);
		drawFrame = 0.0f;
	}*/

	DeleteGameObject();
	
	//シーンを変更するならUpdateの一番最後にかけ
	if (GetUIObject<Fade>(2)->GetOnDraw() == true)
	{
		if (GetUIObject<Fade>(2)->GetFadeState() == None)
		{
			
			Uninit();
			Scene::GetInstance()->ChangeSceneState(new GameScene);
			return;
		}
	}

}

void TitleScene::Draw()
{

	Renderer::Begin();

	for (int i = 0; i < 3; i++) {
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
	

	Renderer::End();

}
