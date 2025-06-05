#include "main.h"
#include "player.h"
#include "transform3DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "PlayerCamera.h"
#include "boxColiderComponent.h"
#include "input.h"
#include "audio.h"
#include "resultScene.h"
#include "meshField.h"
#include "transform3DAnimationComponent.h"
#include "capsuleColiderComponent.h"
#include "slashFirst.h"
#include "slashSecound.h"
#include "circle2DColider.h"
#include "playerIdleState.h"
#include "playerMoveState.h"
#include "playerJumpState.h"
#include "playerAttack1State.h"
#include "playerAttack2State.h"
#include "playerSkill1State.h"
#include "timer.h"

void Player::Init()
{
	AddComponent<Transform3DAnimationComponent>();

	/*GetComponent<Transform3DAnimationComponent>()->AddModelData("asset\\Model\\Akai.fbx");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Akai_Idle.fbx","Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Akai_Run.fbx","Run");*/

	GetComponent<Transform3DAnimationComponent>()->AddModelData("asset\\Model\\PlayerTest.fbx");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Idle.fbx", "Idle");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\RunTest.fbx", "Run");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Slash1.fbx", "Slash1");
	GetComponent<Transform3DAnimationComponent>()->AddAnimationData("asset\\Model\\Slash2.fbx", "Slash2");


	GetComponent<Transform>()->SetSize(XMFLOAT3(0.01f, 0.01f, 0.01f));

	/*GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");*/

	AddComponent<CapsuleColiderComponent>();
	GetComponent<Colider>()->SetSize(XMFLOAT3(0.5f, 0.5f, 0.5f));
	GetComponent<CapsuleColiderComponent>()->SetCenterPosLnegth(0.5f);

	//コライダーを描画
	//GetComponent<CapsuleColiderComponent>()->OnDraw();
	//デバッグを表示
	/*GetComponent<CapsuleColiderComponent>()->OnImGui();*/

	AddComponent<Audio>()->InitMaster();
	GetComponent<Audio>()->Load("asset\\Audio\\wan.wav");

	//このオブジェクトは動的
	m_GameObjectKind = Dynamic;
	m_Weight = 10;

	Velocity = 1.0f;

	m_CildModel = new ModelRenderer(this);
	m_CildModel->Load("asset\\Model\\Box.obj");

	//HPとMP
	m_States =
	{
		500.0f,
		50.0f,
		false,
		0
	};

	Tag = "Player";

	RegisterState(StateType::Idle, std::make_shared<PlayerIdleState>());
	RegisterState(StateType::Move, std::make_shared<PlayerMoveState>());
	RegisterState(StateType::Jump, std::make_shared<PlayerJumpState>());
	RegisterState(StateType::Attack1, std::make_shared<PlayerAttack1State>());
	RegisterState(StateType::Attack2, std::make_shared<PlayerAttack2State>());
	RegisterState(StateType::Skill1, std::make_shared<PlayerSkill1State>());

	//アイドル状態にしておく
	ChangeState(StateType::Idle);
	m_PlayerTime = new Timer;

}

void Player::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void Player::Update()
{
	m_PlayerTime->Update();

	if (m_HitStop == true)
	{
		m_HitStopSecond += m_PlayerTime->GetDeltaTime();
		if (m_HitStopSecond >= 0.05f)
		{
			m_HitStop = false;
			m_HitStopSecond = 0.0f;
		}
		else return;
	}

	ComponentListSort();

	//プレイヤー操作
	PlayerControl();

	//プレイヤーの当たり判定と移動の反映
	PlayerColision();

	//プレイヤーを動かしてから各コンポーネントのアップデート
	for (auto com : ComponentList)
	{
		com->Update();
	}


	//MP自動回復
	if (m_States.MP < 50.0f)
	{
		m_States.MP += 0.01f;
	}
	if (m_States.MP > 50.0f)
	{
		m_States.MP = 50.0f;
	}
	

}

void Player::Draw()
{
	for (auto com : ComponentList)
	{
		com->Draw();
	}

	DrawImGui();

	//XMMATRIX chaildScale, chaildRot, childTrans, childWorld;
	//
	//chaildRot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	//chaildScale = XMMatrixScaling(1.0f / m_Size.x, 1.0f / m_Size.y, 1.0f / m_Size.z);
	//childTrans = XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	//childWorld = childTrans * chaildScale *  world;
	////子のワールド　＝子のローカル * 親のワールド

	//Renderer::SetWorldMatrix(childWorld);

	/*m_CildModel->Draw();*/
}

void Player::PlayerControl()
{
	XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	XMFLOAT3 TransRot = GetComponent<Transform>()->GetRotation();
	XMFLOAT3 pos = GetComponent<Colider>()->GetPosition();
	XMFLOAT3 rot = GetComponent<Colider>()->GetRotation();
	XMFLOAT3 vel = GetComponent<Transform>()->GetVelocity();

	float posRot = GetComponent<Transform>()->GetRot();
	XMFLOAT3 cameraRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<PlayerCamera>()->GetComponent<Transform>()->GetRotation();
	float PI = 3.141592;

	if (Input::GetInstance()->GetKeyTrigger(VK_SPACE))
	{
		ChangeState(StateType::Attack1);
	}

	if (sonick == false) {

		if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || GetKeyState('A') & 0x8000 || GetKeyState('D') & 0x8000)
		{
			ChangeState(StateType::Move);
		}
		else {
			ChangeState(StateType::Idle);
		}

	}


	//ジャンプ処理
	if (GetKeyState('Q') & 0x8000 && m_IsGravity == false) {

		ChangeState(StateType::Jump);
		GetComponent<Audio>()->Play(false);

	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		ChangeState(StateType::Skill1);
	}
	
	
	//重力処理
	if (m_CurrentState->GetName() == StateType::Jump)
	{
		m_IsGravity = false;
	}
	if (m_IsGravity == true) {
		TransPos.y -= m_GravityScale;
		m_GravityScale += 0.01f;
	}
	else {
		m_GravityScale = 0.05f;
	}

	if (Input::GetInstance()->GetKeyTrigger('G'))
	{
		m_IsGravity = !m_IsGravity;
	}

	GetComponent<Colider>()->SetPosition(TransPos);
	GetComponent<Colider>()->SetRotation(rot);
	GetComponent<Colider>()->SetVelocity(vel);
	//GetComponent<CapsuleColiderComponent>()->MoveCollision();

	GetComponent<Transform>()->SetPosition(TransPos);
	GetComponent<Transform>()->SetRotation(rot);
	GetComponent<Transform>()->SetVelocity(vel);


	m_CurrentState->Update();

	
}

void Player::PlayerColision()
{
	////問題点
	////1 カメラバグることあるから注意速攻戻せ
	Transform* trans = GetComponent<Transform>();
	Colider* col = GetComponent<Colider>();

	XMFLOAT3 TransPos = trans->GetPosition();
	XMFLOAT3 TransRot = trans->GetRotation();
	XMFLOAT3 pos = col->GetPosition();
	XMFLOAT3 rot = col->GetRotation();
	float posRot = trans->GetRot();

	float hight = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<MeshField>()->GetHeight(pos);

	
	if (NoCollicion == false)
	{
		if (col->GetIsCollision() == true)
		{
			m_IsGravity = true;
			if(m_CurrentState->GetName() != StateType::Jump)
			{
				if (TransPos.y <= hight + 0.1f) {
					TransPos.y = hight;
					m_IsGravity = false;
				}
			}

			trans->SetPosition(TransPos);
			trans->SetRotation(rot);
			trans->SetRot(posRot);
		}
		else 
		{
		
			m_IsGravity = true;

			if (m_CurrentState->GetName() != StateType::Jump) 
			{
				if (TransPos.y <= hight + 0.1f) {
					TransPos.y = hight;
					m_IsGravity = false;
				}
			}
			else 
			{
				m_IsGravity = false;
			}

			trans->SetPosition(TransPos);
			trans->SetRotation(rot);
			trans->SetRot(posRot);
		}
	

	}

	

	

	//if (GetComponent<Colider>()->GetColiderTag() == "Capsule")
	//{
	//	std::tuple<bool, GameObject*, std::list<GameObject*>> list = GetComponent<CapsuleColiderComponent>()->GetCollision();
	//	//GetCollisionを2改読んじゃってるから直しましょう
	//	if (std::get<0>(list))
	//	{
	//		/*if (NoCollicion == false)
	//		{*/
	//			std::list<GameObject*> objs = std::get<2>(list);

	//			for (auto onCollisionObj : objs)
	//			{

	//				if (onCollisionObj->GetComponent<Colider>()->GetColiderTag() == "Box")
	//				{
	//					/*if (GetComponent<CapsuleColiderComponent>()->GetCorrection().y > 0.0f)
	//					{
	//						isGravity = false;
	//					}*/
	//					XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	//					XMFLOAT3 pos = GetComponent<Colider>()->GetPosition();

	//					if (isJump == false) {
	//						if (TransPos.y <= hight + 0.1f) {
	//							TransPos.y = hight;
	//							m_IsGravity = false;
	//						}
	//					}

	//					//XMFLOAT3 velocity = GetComponent<Colider>()->GetVelocity();
	//					////コライダーのポジション更新
	//					
	//					GetComponent<Transform>()->SetPosition(TransPos);
	//					GetComponent<Transform>()->SetRotation(rot);
	//					GetComponent<Transform>()->SetRot(posRot);

	//				}
	//				if (onCollisionObj->GetComponent<Colider>()->GetColiderTag() == "Capsule")
	//				{
	//					////コライダーのポジション更新
	//					//XMFLOAT3 pos = GetComponent<Colider>()->GetPosition();

	//					//GetComponent<Transform>()->SetPosition(pos);
	//					//GetComponent<Transform>()->SetRotation(rot);
	//					//GetComponent<Transform>()->SetRot(posRot);
	//				}
	//			}
	//		//}
	//	}
	//	else {
	//		m_IsGravity = true;

	//		if (isJump == false) {
	//			if (TransPos.y <= hight + 0.1f) {
	//				TransPos.y = hight;
	//				m_IsGravity = false;
	//			}
	//		}

	//		GetComponent<Transform3DAnimationComponent>()->SetPosition(TransPos);
	//		GetComponent<Transform3DAnimationComponent>()->SetRotation(rot);
	//		GetComponent<Transform3DAnimationComponent>()->SetRot(posRot);
	//	}
	//}

}
float currentTime = 0.0f;

void Player::DrawImGui()
{

	XMFLOAT3 TransPos = GetComponent<Transform>()->GetPosition();
	XMFLOAT3 TransRot = GetComponent<Transform>()->GetRotation();
	XMFLOAT3 ColiderPos = GetComponent<Colider>()->GetPosition();
	XMFLOAT3 CroiderRot = GetComponent<Colider>()->GetRotation();
	currentTime += m_PlayerTime->GetDeltaTime();

	{

		ImguiManager* imgui = Scene::GetInstance()->GetScene<GameScene>()->GetImguiManager();

		ImGui::Begin("Player");                          // Create a window called "Hello, world!" and append into it.              // Display some text (you can use a format strings too)

		static bool draw;

		ImGui::Checkbox("DrawColider", &draw);

		GetComponent<CapsuleColiderComponent>()->CapsuleDrawState(draw);

		ImGui::Text("playerPos , x = %.1f , y = %.1f , z = %.1f", TransPos.x, TransPos.y, TransPos.z);

		static  float add[3] = {0.0f,0.0f,0.0f};

		ImGui::SliderFloat3("AddPos", add, -1.0f, 1.0f);

		GetComponent<Colider>()->SetAddPosition(XMFLOAT3(add[0],add[1],add[2]));

		ImGui::Text("PlayerHP = %.1f", m_States.HP);

		ImGui::Text("FPS = %.1f", imgui->FrameRate);

		ImGui::Text("deltaTime = %.1f", currentTime);

		ImGui::End();

	}

}




