#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "titleBoard.h"
#include "texturePool.h"
#include "titleCamera.h"
#include "titleScene.h"
#include "pushToSpaceBoard.h"

void TitleBoard::Init()
{

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-4.0f, 3.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(-4.0f, 3.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-4.0f, 0.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(-4.0f, 0.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(0.0f, 1.0f);

	AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("TitleName"));
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	


	for (auto com : ComponentList) {
		com->Init();
	}

	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(20.0f, 20.0f, 20.0f));

}

void TitleBoard::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void TitleBoard::Update()
{

	float speed = 0.02f;

	if (Scene::GetInstance()->GetScene<TitleScene>()->GetGameObject<TitleCamera>()->GetMoveState() == true)
	{
		//アニメーション
		{

			VERTEX_3D vertex[4];
			for (int i = 0; i < 4; i++) {
				vertex[i] = GetComponent<Transform2DComponent>()->GetVertex(i);
			}
			if (vertex[1].Position.x <= 4.0f)
			{

				vertex[1].Position.x = vertex[1].Position.x + speed;
				vertex[3].Position.x = vertex[3].Position.x + speed;

				vertex[1].TexCoord.x = vertex[1].TexCoord.x + speed / 8;
				vertex[3].TexCoord.x = vertex[3].TexCoord.x + speed / 8;


				/*if (vertex[1].Position.x >= 4.0f)
				{
					vertex[1].Position.x = -4.0f;
					vertex[3].Position.x = -4.0f;

					vertex[1].TexCoord.x = 0.0f;
					vertex[3].TexCoord.x = 0.0f;
				}*/

				GetComponent<Transform2DComponent>()->SetVertex(vertex);
			}
			else if (doOnce == false)
			{
				Scene::GetInstance()->GetScene<TitleScene>()->AddGameObject<PushToSpaceBoard>(1)->GetComponent<Transform>()->SetPosition(XMFLOAT3(0.0f, 9.0f, 100.0f));
				doOnce = true;
			}
		}

	}
	DeleteRecest();

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void TitleBoard::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
