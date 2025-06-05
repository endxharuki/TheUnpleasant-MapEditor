#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "textDamage.h"
#include "texturePool.h"

void TextDamage::Init()
{

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-3.0f, 3.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(3.0f, 3.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-3.0f, -3.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(3.0f, -3.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("Number"));
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	GetComponent<Transform2DComponent>()->SetIsBilboad(true);

	for (auto com : ComponentList) {
		com->Init();
	}

	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(1.0f, 1.0f, 1.0f));

}

void TextDamage::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void TextDamage::Update()
{

	XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
	pos.y += 0.05f;
	GetComponent<Transform>()->SetPosition(pos);

	//アニメーション
	{

		VERTEX_3D vertex[4];
		for (int i = 0;i < 4;i++) {
			vertex[i] = GetComponent<Transform2DComponent>()->GetVertex(i);
		}

		float x = 1.0f / 5.0f;
		float y = 1.0f / 5.0f;
		float TexX = textUV % 5 * x;
		float TexY = textUV / 5 * y;

		vertex[0].TexCoord = XMFLOAT2(TexX , TexY);
		vertex[1].TexCoord = XMFLOAT2(TexX + x, TexY);
		vertex[2].TexCoord = XMFLOAT2(TexX, TexY + y);
		vertex[3].TexCoord = XMFLOAT2(TexX + x, TexY + y);
		GetComponent<Transform2DComponent>()->SetVertex(vertex);

		frameCount++;
	}

	if (frameCount >= 16.0f) isDelete = true;

	DeleteRecest();

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void TextDamage::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
