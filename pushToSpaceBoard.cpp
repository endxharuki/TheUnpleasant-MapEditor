#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "pushToSpaceBoard.h"
#include "texturePool.h"

void PushToSpaceBoard::Init()
{

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-4.0f, 3.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(4.0f, 3.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-4.0f, 0.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(4.0f, 0.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("PushSpace"));
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	


	for (auto com : ComponentList) {
		com->Init();
	}

	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(8.0f, 8.0f, 8.0f));

}

void PushToSpaceBoard::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void PushToSpaceBoard::Update()
{
	if (frameCount >= 80) frameCount = 0;
	else if (frameCount >= 40) GetComponent<Transform>()->SetDraw(true);
	else if (frameCount >= 0) GetComponent<Transform>()->SetDraw(false);

	frameCount++;

	DeleteRecest();

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void PushToSpaceBoard::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
