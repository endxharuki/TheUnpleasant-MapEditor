#include "main.h"
#include "enemy.h"
#include "player.h"
#include "scene.h"
#include "gameScene.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "EnemyAIController.h"
#include "capsuleColiderComponent.h"
#include "meshField.h"
#include "transform3DAnimationComponent.h"

void Enemy::Init()
{
	AddComponent<Transform3DAnimationComponent>();


	GetComponent<Transform3DAnimationComponent>()->AddModelData("asset\\Model\\Cyclops.fbx");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\CyclopsIdle.fbx", "Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\CyclopsWalk.fbx", "Walk");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\CyclopsDied.fbx", "Died");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\CyclopsAttack.fbx", "Attack");


	//アニメーションの速度をどのぐらい落とすか
	GetComponent<Transform3DAnimationComponent>()->SetDelay(2);

	Tag = "Enemy";

	m_GameObjectKind = Dynamic;
	m_Weight = 10;

	AddComponent<CapsuleColiderComponent>()->SetSize(XMFLOAT3(2.0f,2.0f,2.0f));
	GetComponent<CapsuleColiderComponent>()->SetCenterPosLnegth(1.5f);

	GetComponent<CapsuleColiderComponent>()->OnDraw();

	AddComponent<EnemyAI>()->AddParent(this);
	GetComponent<EnemyAI>()->ChangeState(Idle,this);
	
	//HPとMP
	m_States =
	{
		10000.0f,
		0.0f,
		false,
		0
	};
}

void Enemy::Uninit()
{

	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void Enemy::Update()
{

	for (auto com : ComponentList)
	{
		com->Update();

	}

	DeleteComponent();
}

void Enemy::Draw()
{
	
	for (auto com : ComponentList)
	{
		com->Draw();
	}
	if (m_States.HP > 0)
	{
		DrawImGui();
	}
}

void Enemy::DrawImGui()
{
	                        // Create a window called "Hello, world!" and append into it.

	if (GetComponent<Colider>() == nullptr) return;

	XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	XMFLOAT3 TransRot = GetComponent<Transform>()->GetRotation();
	XMFLOAT3 ColiderPos = GetComponent<Colider>()->GetPosition();
	XMFLOAT3 CroiderRot = GetComponent<Colider>()->GetRotation();

	{
		
		ImguiManager* imgui = Scene::GetInstance()->GetScene<GameScene>()->GetImguiManager();

		          // Create a window called "Hello, world!" and append into it.
		static bool draw;

		ImGui::Checkbox("DrawColider", &draw);
		
		GetComponent<CapsuleColiderComponent>()->CapsuleDrawState(draw);

		ImGui::Text("playerPos , x = %.1f , y = %.1f , z = %.1f", TransPos.x, TransPos.y, TransPos.z);

		static  float add[3] = { 0.0f,0.0f,0.0f };

		ImGui::SliderFloat3("AddPos", add, -2.0f, 2.0f);

		GetComponent<Colider>()->SetAddPosition(XMFLOAT3(add[0], add[1], add[2]));

		ImGui::Text("FPS = %.1f", imgui->FrameRate);

		

	}
	

}
