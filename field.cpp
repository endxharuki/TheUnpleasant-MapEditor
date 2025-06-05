#include "main.h"
#include "field.h"
#include "transform2DComponent.h"
#include "audio.h"
#include "texturePool.h"

void Field::Init()
{

	AddComponent<Audio>()->InitMaster();
	GetComponent<Audio>()->Load("asset\\Audio\\game.wav");

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-50.0f, 0.0f, 50.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(50.0f, 0.0f, 50.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-50.0f, 0.0f, -50.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(50.0f, 0.0f, -50.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("Jimen"));
	GetComponent<Transform2DComponent>()->SetVertex(vertex);

	for (auto com : ComponentList)
	{
		com->Init();
	}
	GetComponent<Transform2DComponent>()->SetPosition(XMFLOAT3(0.0f, -5.0f, 0.0f));
	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(1.0f, 1.0f, 1.0f));

	GetComponent<Audio>()->Play(false);

}

void Field::Uninit()
{

	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void Field::Update()
{

	XMFLOAT3 pos = GetComponent<Transform2DComponent>()->GetPosition();

	for (auto com : ComponentList)
	{
		com->Update();

	}

}

void Field::Draw()
{
	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
