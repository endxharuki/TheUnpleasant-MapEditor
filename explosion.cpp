#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "explosion.h"
#include "texturePool.h"

void Explosion::Init()
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

	AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("Explosion"));
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	GetComponent<Transform2DComponent>()->SetIsBilboad(true);

	for (auto com : ComponentList) {
		com->Init();
	}

	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(3.0f, 3.0f, 3.0f));

	//アニメーション
	{

		VERTEX_3D vertex[4];
		for (int i = 0; i < 4; i++) {
			vertex[i] = GetComponent<Transform2DComponent>()->GetVertex(i);
		}

		float x = 1.0f / 4.0f;
		float y = 1.0f / 4.0f;
		float TexX = frameCount % 4 * x;
		float TexY = frameCount / 4 * y;

		vertex[0].TexCoord = XMFLOAT2(TexX, TexY);
		vertex[1].TexCoord = XMFLOAT2(TexX + x, TexY);
		vertex[2].TexCoord = XMFLOAT2(TexX, TexY + y);
		vertex[3].TexCoord = XMFLOAT2(TexX + x, TexY + y);
		GetComponent<Transform2DComponent>()->SetVertex(vertex);

	}

}

void Explosion::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void Explosion::Update()
{
	//ヒットストップ中は処理をしない
	if (m_ParentObject->GetHitStopState() == true) return;
	
	//アニメーション
	{

		VERTEX_3D vertex[4];
		for (int i = 0;i < 4;i++) {
			vertex[i] = GetComponent<Transform2DComponent>()->GetVertex(i);
		}

		float x = 1.0f / 4.0f;
		float y = 1.0f / 4.0f;
		float TexX = frameCount % 4 * x;
		float TexY = frameCount / 4 * y;

		vertex[0].TexCoord = XMFLOAT2(TexX, TexY);
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

void Explosion::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
