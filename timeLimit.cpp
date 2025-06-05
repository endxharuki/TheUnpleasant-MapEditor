#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "texture2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "timeLimit.h"
#include "texturePool.h"

void TimeLimit::Init()
{
	//	TextureNumberには気を付けて
	AddComponent<Texture2DComponent>()->SetPosTexNumber(960.0f, 0.0f, 100.0f, 100.0f, TexturePool::GetInstance()->GetTexture("Number"), 4);
	AddComponent<Texture2DComponent>()->SetPosTexNumber(1200.0f, 0.0f, 100.0f, 100.0f, TexturePool::GetInstance()->GetTexture("Number"), 3);
	AddComponent<Texture2DComponent>()->SetPosTexNumber(1120.0f, 0.0f, 100.0f, 100.0f, TexturePool::GetInstance()->GetTexture("Number"), 2);
	AddComponent<Texture2DComponent>()->SetPosTexNumber(1040.0f, 0.0f, 100.0f, 100.0f, TexturePool::GetInstance()->GetTexture("Number"),1);

	for (auto com : ComponentList)
	{
		com->Init();
	}

}

void TimeLimit::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void TimeLimit::Update()
{
	count++;
	if(count ==60)
	{
		second[0]--;
		count = 0;
	}
	if (second[0] == -1) {
		second[0] = 9;
		second[1]--;
	}
	if (second[1] == -1) {
		second[1] = 9;
		second[2]--;
	}
	//アニメーション
	{//マーク
		int second = 12;
		float x = 1.0f / m_AnimetionX;
		float y = 1.0f / m_AnimetionY;
		float TexX = second % (int)m_AnimetionX * x;
		float TexY = second / (int)m_AnimetionY * y;

		GetComponent<Texture2DComponent>(4)->SetAnimation(TexX, TexY, x, y);

	}
	//3桁の処理
	for (int i = 2; i >= 0;i--)
	{
		int n = 3 - i;

		float x = 1.0f / m_AnimetionX;
		float y = 1.0f / m_AnimetionY;
		float TexX = second[i] % (int)m_AnimetionX * x;
		float TexY = second[i] / (int)m_AnimetionY * y;

		GetComponent<Texture2DComponent>(n)->SetAnimation(TexX, TexY, x, y);

	}

	//if (count == 10) count = 0;

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void TimeLimit::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
