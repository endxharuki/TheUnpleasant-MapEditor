#include "main.h"
#include "player.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "textrue2D.h"
#include "renderer.h"
#include "texture2DComponent.h"
#include "texturePool.h"

void Texture2D::Init()
{
	AddComponent<Texture2DComponent>()->SetPosTexNumber(posX, posY, sizeX, sizeY, m_Texture, number);
	
	for (auto com : ComponentList)
	{
		com->Init();
	}
}

void Texture2D::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void Texture2D::Update()
{

	//アニメーション

	//if (frameCount >= 16.0f) isDelete = true;
	GetComponent<Texture2DComponent>(number)->SetPosTexNumber(posX, posY, sizeX, sizeY, m_Texture, number);
	GetComponent<Texture2DComponent>(number)->SetAnimation(0.0f, 0.0f, 1.0f, 1.0f);

	DeleteRecest();

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void Texture2D::Draw()
{

	if (OnDraw == true) {
		//入力処理
		for (auto com : ComponentList)
		{
			com->Draw();
		}

	}
}

void Texture2D::SetVertex(VERTEX_3D Vertex[4])
{
}

