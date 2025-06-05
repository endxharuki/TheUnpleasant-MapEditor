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

void TitleScene::Init()
{

	audio = new Audio;

	audio->InitMaster();
	audio->Load("asset\\Audio\\title.wav");

	Renderer::Init();
	//座標とテクスチャとナンバーを設定

	//座標とテクスチャとナンバーを設定
	AddTexture<Texture2D>()->SetPosTexNumber(0.0f, 0.0f, 1300.0f, 730.0f, L"asset\\Texture\\title_4.png", 1);
	AddTexture<Texture2D>()->SetPosTexNumber(420.0f, 500.0f, 500.0f, 180.0f, L"asset\\Texture\\push.png", 2);
	AddTexture<Texture2D>()->SetPosTexNumber(300.0f, 200.0f, 800.0f, 200.0f, L"asset\\Texture\\gametitle_1.png", 3);


	for (auto tex : m_TextureList)
	{

		tex->Init();
	}

	
	//レイヤー順番バカ大事だから気を付けて
	audio->Play(false);

}

void TitleScene::Uninit()
{
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Uninit();
			delete obj;
		}

		m_GameObjectList[i].clear();
	}
	for (auto tex : m_TextureList) {
		tex->Uninit();
		delete tex;
	}
	Renderer::Uninit();

	audio->Uninit();
	delete audio;

}

void TitleScene::Update()
{
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Update();
		}
	}
	for (auto tex : m_TextureList) {
		tex->Update();
		
	}

	drawFrame++;
	if (drawFrame >= 40.0f)
	{

		GetTexture(2)->SetOnDraw(false);
	}
	if (drawFrame >= 80.0f)
	{

		GetTexture(2)->SetOnDraw(true);
		drawFrame = 0.0f;
	}

	DeleteGameObject();
	//シーンを切り替える前にDeletegameObjectを呼ばないとエラー出る
	if (GetKeyState(VK_SPACE) & 0x8000) {
		
		Uninit();
		Scene::GetInstance()->ChangeSceneState(new GameScene);
	}

}

void TitleScene::Draw()
{

	Renderer::Begin();
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i])
		{
			obj->Draw();
		}
	}
	for (auto tex : m_TextureList) {
		tex->Draw();
	}
	Renderer::End();

}
