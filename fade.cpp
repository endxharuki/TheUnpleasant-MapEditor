#include "main.h"
#include "player.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "fade.h"
#include "renderer.h"
#include "texture2DComponent.h"
#include "texturePool.h"

void Fade::Init()
{
	AddComponent<Texture2DComponent>()->SetPosTexNumber(posX, posY, sizeX, sizeY, m_Texture, 0);
	
	for (auto com : ComponentList)
	{
		com->Init();
	}
}

void Fade::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void Fade::Update()
{

	//アニメーション
	if (m_FadeState == Out)
	{
		m_Alpha += 0.01f;
		GetComponent<Texture2DComponent>(0)->SetAlpha(m_Alpha);
		if (m_Alpha >= 1.0f) {
			m_FadeState = None;
		}
	}
	else if (m_FadeState == In)
	{
		m_Alpha -= 0.01f;
        GetComponent<Texture2DComponent>(0)->SetAlpha(m_Alpha);
		if (m_Alpha <= 0.0f) m_FadeState = None;

	}

	

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void Fade::Draw()
{

	if (OnDraw == true) {
		//入力処理
		for (auto com : ComponentList)
		{
			com->Draw();
		}

	}
}

