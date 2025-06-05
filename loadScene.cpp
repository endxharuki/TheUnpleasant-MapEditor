#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "loadScene.h"
#include "gameScene.h"
#include "scene.h"
#include "enemy.h"
#include "block.h"
#include "explosion.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "titleScene.h"
#include "texturePool.h"

void LoadScene::Init()
{

	AddTexture<Texture2D>();

	for (auto tex : m_TextureList)
	{

		tex->Init();
	}

}

void LoadScene::Uninit()
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

void LoadScene::Update()
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
		Scene::GetInstance()->ChangeSceneState(new TitleScene);
	}

}

void LoadScene::Draw()
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
