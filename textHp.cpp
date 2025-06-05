#include "main.h"
#include "transform2DComponent.h"
#include "texture2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "textHp.h"
#include "texturePool.h"


void TextHp::Init()
{
	//	TextureNumber‚É‚Í‹C‚ð•t‚¯‚Ä

	AddComponent<Texture2DComponent>()->SetPosTexNumber(30.0f, 30.0f, 500.0f, 30.0f, TexturePool::GetInstance()->GetTexture("HpFlame"), 0);
	AddComponent<Texture2DComponent>()->SetPosTexNumber(30.0f, 30.0f, 500.0f, 30.0f, TexturePool::GetInstance()->GetTexture("Hp"), 1);
	m_Player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
	if (m_Player != nullptr)
	{
		firstHp = 500.0f / m_Player->GetStates()->HP;
	}
	

	for (auto com : ComponentList)
	{
		com->Init();
	}

}

void TextHp::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void TextHp::Update()
{
	Texture2DComponent* tex = GetComponent<Texture2DComponent>(1);
	
	float hp = firstHp * m_Player->GetStates()->HP;

	tex->SetTexturePosSize(30.0f, 30.0f, hp, 30.0f);
	
	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void TextHp::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
