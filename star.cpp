#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "star.h"
#include "texturePool.h"
#include "sphereColiderComponent.h"
#include "titleScene.h"


void Star::Init()
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

	AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("Star"));
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	GetComponent<Transform2DComponent>()->SetIsBilboad(true);

	for (auto com : ComponentList) {
		com->Init();
	}

	GetComponent<Transform2DComponent>()->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(2.0f, 2.0f, 2.0f));
	/*GetComponent<Transform2DComponent>()->SetVelocity(XMFLOAT3(0.3f, 0.2f, 0.5f));*/


	AddComponent<SphereColiderComponent>()->SetSize(XMFLOAT3(1.0f, 1.0f, 1.0f));
	GetComponent<SphereColiderComponent>()->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));

	m_Particle = Scene::GetInstance()->GetScene<TitleScene>()->AddGameObject<ParticleSpowner>(1);


}

void Star::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void Star::Update()
{

	

	XMFLOAT3 pos = GetComponent<Colider>()->GetPosition();
	XMFLOAT3 vel = GetComponent<Transform>()->GetVelocity();

	pos = { pos.x + vel.x, pos.y + vel.y, pos.z + vel.z};

	GetComponent<Colider>()->SetPosition(pos);
	GetComponent<Transform>()->SetPosition(pos);
	m_Particle->SetPosition(pos);

	for (auto com : ComponentList)
	{
		com->Update();
	}


	//if (GetComponent<Colider>()->GetColiderTag() == "Sphere")
	//{
	//	std::tuple<bool, GameObject*, std::list<GameObject*>> list = GetComponent<SphereColiderComponent>()->GetCollision();
	//	//GetCollision‚ð2‰ü“Ç‚ñ‚¶‚á‚Á‚Ä‚é‚©‚ç’¼‚µ‚Ü‚µ‚å‚¤
	//	if (std::get<0>(list))
	//	{
	//		std::list<GameObject*> objs = std::get<2>(list);

	//		for (auto onCollisionObj : objs)
	//		{
	//			if (onCollisionObj->GetComponent<Colider>()->GetColiderTag() == "Box")
	//			{

	//				

	//			}
	//		}
	//	}

	//}
	

	
	


	DeleteRecest();

	
}

void Star::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}
