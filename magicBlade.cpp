#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "magicBlade.h"
#include "texturePool.h"
#include "capsuleColiderComponent.h"
#include "textDamage.h"
#include <random>
#include "playerCamera.h"
#include "circle2DColider.h"
#include "explosion.h"



void MagicBlade::Init()
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
	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(0.7f, 0.7f, 0.7f));

	length = 3.0f;

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 playerRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();
	XMFLOAT3 texpos = { playerPos.x + (length * sinf(playerRot.y)),playerPos.y + 1.5f ,playerPos.z + (length * cosf(playerRot.y)) };
	//カプセルの位置
	XMFLOAT3 capsulePos = { playerPos.x + (length * sinf(playerRot.y)),playerPos.y + 1.5f ,playerPos.z + (length * cosf(playerRot.y)) };

	//プレイヤーの右側のベクトル
	XMFLOAT3 playerRight = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRight();

	capsulePos = { capsulePos.x + playerRight.x * 0.2f,capsulePos.y ,capsulePos.z + playerRight.z * 0.2f };
	texpos = { capsulePos.x + playerRight.x * 0.2f,capsulePos.y ,capsulePos.z + playerRight.z * 0.2f };

	AddComponent<CapsuleColiderComponent>()->SetTransform(capsulePos,XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.5f, 1.0f, 0.5f));
	GetComponent<CapsuleColiderComponent>()->NoUpdate();

	GetComponent<CapsuleColiderComponent>()->SetRotationAngle(XMFLOAT3(90.0f,0.0f,0.0f));
	XMFLOAT3 r = GetComponent<CapsuleColiderComponent>()->GetRotation();

	GetComponent<CapsuleColiderComponent>()->SetRotation(XMFLOAT3(r.x,playerRot.y,r.z));


	XMFLOAT3 vector = { capsulePos.x - playerPos.x,(capsulePos.y - playerPos.y) - 1.0f,capsulePos.z - playerPos.z };

	//vector = Normalize(vector, length);
	float PI = 3.141592;

	////斜めにする
	float Rot = PI / 4;


	//球座標タイプ
	GetComponent<Colider>()->RotateAroundInit(playerPos);
	//GetComponent<Colider>()->RotateAround(0.0f, -Rot);
	

	XMFLOAT3 p = GetComponent<Colider>()->GetPosition();
	
	p = GetComponent<Colider>()->GetRotatePosition(playerPos);

	GetComponent<CapsuleColiderComponent>()->SetPosition(p);

	
	//カプセルの設定
	GetComponent<CapsuleColiderComponent>()->SetCenterPosLnegth(1.0f);

	//カプセルを設置
	GetComponent<CapsuleColiderComponent>()->MoveCollision();

	GetComponent<CapsuleColiderComponent>()->DrawImGui();

	GetComponent<CapsuleColiderComponent>()->OnDraw();


	GetComponent<Transform2DComponent>()->SetPosition(texpos);

	//ロットの初期化
	rot = 0.0f;

	m_AttackPoint = 880;

	Tag = "Attack";

}

void MagicBlade::Uninit()
{
	for (auto obj : m_HitObjectList)
	{
		obj->SetHit(false);
		obj->SetHitCount(0);
	}

	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void MagicBlade::Update()
{

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 playerRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();
	XMFLOAT3 playerVec = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetFoward();

	/*XMFLOAT3 capsulePos = GetComponent<Colider>()->GetPosition();*/
	if (m_OnBurst == false)
	{
		float PI = 3.141592;
		XMFLOAT3 texpos = { playerPos.x + (length * sinf(playerRot.y)),playerPos.y + 1.5f ,playerPos.z + (length * cosf(playerRot.y)) };
		XMFLOAT3 capsulePos = { playerPos.x + (length * sinf(playerRot.y)),playerPos.y + 1.5f ,playerPos.z + (length * cosf(playerRot.y)) };

		//プレイヤーの右側のベクトル
		XMFLOAT3 playerRight = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRight();

		capsulePos = { capsulePos.x + playerRight.x * 0.2f,capsulePos.y ,capsulePos.z + playerRight.z * 0.2f };
		texpos = { capsulePos.x + playerRight.x * 0.2f,capsulePos.y ,capsulePos.z + playerRight.z * 0.2f };


		GetComponent<Colider>()->SetPosition(capsulePos);

		float rotX = (PI / 180.0f) * 90.0f;

		GetComponent<CapsuleColiderComponent>()->SetRotation(XMFLOAT3(rotX, playerRot.y, 0.0f));
		GetComponent<Transform2DComponent>()->SetPosition(texpos);

		//見ている敵のポジションを取る
		XMFLOAT3 targetPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<PlayerCamera>()->GetComponent<Circle2DColider>()->GetTragetWorldPosition();

		m_TargetVector = { targetPos.x - playerPos.x,targetPos.y - playerPos.y ,targetPos.z - playerPos.z };
		m_TargetVector = Normalize(m_TargetVector);

		GetComponent<CapsuleColiderComponent>()->MoveCollision();

	}

	if (m_OnBurst == true)
	{
		float moveSpeed = 1.0f;

		XMFLOAT3 texpos = GetComponent<Transform>()->GetPosition();
		XMFLOAT3 capsulePos = GetComponent<Colider>()->GetPosition();
		//飛ばす
		texpos = { texpos.x + (m_TargetVector.x) * moveSpeed,texpos.y + (m_TargetVector.y * moveSpeed) ,texpos.z + (m_TargetVector.z * moveSpeed) };
		capsulePos = { capsulePos.x + (m_TargetVector.x) * moveSpeed,capsulePos.y + (m_TargetVector.y * moveSpeed) ,capsulePos.z + (m_TargetVector.z * moveSpeed) };


		GetComponent<Transform2DComponent>()->SetPosition(texpos);
		GetComponent<CapsuleColiderComponent>()->SetPosition(capsulePos);

		GetComponent<CapsuleColiderComponent>()->MoveCollision();
		frameCount++;
	}

	if (frameCount >= 60.0f) isDelete = true;


	HitAttackCollision();

	DeleteRecest();

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void MagicBlade::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}

void MagicBlade::HitAttackCollision()
{
	XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();

	std::tuple<bool, GameObject*, std::list<GameObject*>> date = GetComponent<CapsuleColiderComponent>()->GetCollision();

	if (std::get<0>(date) == true)
	{
		std::list<GameObject*> objList = std::get<2>(date);

		bool hit = false;

		for (auto obj : objList)
		{
			if (obj->GetTag() == "Enemy")
			{
				if (obj->GetStates()->hit == false)
				{
					
					/*float hp = obj->GetStates().HP;
					hp -= 1.0f;
					obj->SetStates(hp, 0.0f, true,count);*/
					
					Explosion* ex = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<Explosion>(1);;
					ex->AddParentGameObject(m_ParentObject);
					ex->GetComponent<Transform>()->SetPosition(pos);
					

					int count = obj->GetStates()->hitCount;
					count++;

					obj->SetHitCount(count);
					if (count >= 2) obj->SetHit(true);
					m_HitObjectList.push_back(obj);

					XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
					XMFLOAT3 playerRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();

					pos.y += 2.0f;

					XMFLOAT3 anglePos = GetAddAnglePosition(playerRot, XMFLOAT3(1.0f, 0.0f, 0.0f));
					anglePos = { anglePos.x * 0.5f,anglePos.y * 0.5f ,anglePos.z * 0.5f };
					
					int digits = std::log10(m_AttackPoint);

					digits++;

					//HPを減らす
					float hp = obj->GetStates()->HP;
					hp -= m_AttackPoint;
					obj->SetHp(hp);

					//ダメージ表示を生成
					TextAttackPoint(digits);

					hit = true;

				}
			}

			if (hit == true) m_HitCount++;
			
		}
	}


}

void MagicBlade::TextAttackPoint(int n)
{

	//プレイヤー情報取得
	Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
	Transform* playerTrans = player->GetComponent<Transform>();
	XMFLOAT3 playerRot = playerTrans->GetRotation();
	XMFLOAT3 playerFoward = playerTrans->GetFoward();
	XMFLOAT3 playerRight = playerTrans->GetRight();

	float textToWiedth = 0.8f;


	XMFLOAT3 anglePos = GetComponent<Colider>()->GetPosition();
	XMFLOAT3 textPos = { anglePos.x - playerFoward.x,anglePos.y - playerFoward.y ,anglePos.z - playerFoward.z };


	TextDamage* text[4];

	int num = 0;

	for (int i = 0; i < n; i++)
	{
		if (i == 0) num = m_AttackPoint % 10;
		if (i == 1) num = (m_AttackPoint / 10) % 10;
		if (i == 2) num = (m_AttackPoint / 100) % 10;
		if (i == 3) num = m_AttackPoint / 1000;

		text[i] = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<TextDamage>(1);
		text[i]->GetComponent<Transform>()->SetTransform(
			textPos, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.2f, 0.2f, 0.2f));
		text[i]->SetTextUV(num);

		textPos = { textPos.x - (playerRight.x * textToWiedth),textPos.y /*- anglePos.y*/,textPos.z - (playerRight.z * textToWiedth) };

	}

	
}
