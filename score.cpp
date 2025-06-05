#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "texture2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "score.h"
#include "texturePool.h"

void Score::Init()
{
	AddComponent<Texture2DComponent>()->SetPosTexNumber(0.0f, 0.0f, 100.0f, 100.0f, TexturePool::GetInstance()->GetTexture("Number"),1);
}

void Score::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void Score::Update()
{
	//アニメーション
	{

		float x = 1.0f / m_AnimetionX;
		float y = 1.0f / m_AnimetionY;
		float TexX = m_Score % (int)m_AnimetionX * x;
		float TexY = m_Score / (int)m_AnimetionY * y;

		GetComponent<Texture2DComponent>(1)->SetAnimation(TexX, TexY, x, y);

	}

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void Score::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
