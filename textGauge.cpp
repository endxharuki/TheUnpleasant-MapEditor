#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "texture2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "textGauge.h"
#include "texturePool.h"

void TextGauge::Init()
{

	//	TextureNumber‚É‚Í‹C‚ð•t‚¯‚Ä
	AddComponent<Texture2DComponent>()->SetPosTexNumber(30.0f, 40.0f, 450.0f, 100.0f, TexturePool::GetInstance()->GetTexture("StaminaFlame"), 0);
	AddComponent<Texture2DComponent>()->SetPosTexNumber(30.0f, 40.0f, 450.0f, 100.0f, TexturePool::GetInstance()->GetTexture("Stamina"), 1);

	firstMp = (450.0f / Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetStates()->MP);

	for (auto com : ComponentList)
	{
		com->Init();
	}

}

void TextGauge::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void TextGauge::Update()
{
	Texture2DComponent* tex = GetComponent<Texture2DComponent>(1);
	float mp = firstMp * Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetStates()->MP;


	tex->SetTexturePosSize(30.0f, 40.0f, mp, 100.0f);
	

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void TextGauge::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
