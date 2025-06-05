#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "slashSecound.h"
#include "texturePool.h"
#include "capsuleColiderComponent.h"
#include "textDamage.h"
#include "explosion.h"

void SlashSecound::Init()
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

	AddComponent<Transform2DComponent>()->AddTextureData(TexturePool::GetInstance()->GetTexture("slashSecound"));
	GetComponent<Transform2DComponent>()->SetVertex(vertex);
	GetComponent<Transform2DComponent>()->SetIsBilboad(true);

	for (auto com : ComponentList) {
		com->Init();
	}
	GetComponent<Transform2DComponent>()->SetSize(XMFLOAT3(1.0f, 1.0f, 1.0f));

	length = 2.0f;

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 playerRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();
	XMFLOAT3 texpos = { playerPos.x + (length * sinf(playerRot.y)),playerPos.y + 1.5f ,playerPos.z + (length * cosf(playerRot.y)) };
	XMFLOAT3 capsulePos = { playerPos.x + (length * sinf(playerRot.y)),playerPos.y + 1.5f ,playerPos.z + (length * cosf(playerRot.y)) };

	AddComponent<CapsuleColiderComponent>()->SetTransform(capsulePos,XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f));
	GetComponent<CapsuleColiderComponent>()->NoUpdate();

	GetComponent<CapsuleColiderComponent>()->SetRotationAngle(XMFLOAT3(90.0f,0.0f,0.0f));
	XMFLOAT3 r = GetComponent<CapsuleColiderComponent>()->GetRotation();

	GetComponent<CapsuleColiderComponent>()->SetRotation(XMFLOAT3(r.x,playerRot.y,r.z));


	XMFLOAT3 vector = { capsulePos.x - playerPos.x,(capsulePos.y - playerPos.y) - 1.0f,capsulePos.z - playerPos.z };

	//vector = Normalize(vector, length);
	float PI = 3.141592;

	////斜めにする
	float Rot = PI / 4;

	//XMFLOAT3 pos = GetComponent<Colider>()->GetAddAnglePosition(XMFLOAT3(0.0f,Rot,0.0f), vector);

	//rot = playerRot.y;

	//pos = { pos.x * length,pos.y * length,pos.z * length };

	//pos = { playerPos.x + pos.x ,playerPos.y + pos.y + 1.0f ,playerPos.z + pos.z };

	//GetComponent<CapsuleColiderComponent>()->SetPosition(pos);


	//球座標タイプ
	GetComponent<Colider>()->RotateAroundInit(playerPos);
	GetComponent<Colider>()->RotateAround(Rot, Rot);
	

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

	m_AttackPoint = 140;

	Tag = "Attack";


}

void SlashSecound::Uninit()
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

void SlashSecound::Update()
{
	//ヒットストップ中は処理をしない
	if (m_ParentObject->GetHitStopState() == true) return;

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 playerRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();

	XMFLOAT3 capsulePos = GetComponent<Colider>()->GetPosition();
	
	float PI = 3.141592;

	//球座標タイプ
	float rotScale = PI * 0.05f;

	XMFLOAT3 p = GetComponent<Colider>()->GetPosition();
	GetComponent<Colider>()->RotateAround(-rotScale, -rotScale);

	p = GetComponent<Colider>()->GetRotatePosition(playerPos);

	GetComponent<CapsuleColiderComponent>()->SetPosition(p);

	GetComponent<CapsuleColiderComponent>()->MoveCollision();

	HitAttackCollision();

	//アニメーション
	{

		VERTEX_3D vertex[4];
		for (int i = 0;i < 4;i++) {
			vertex[i] = GetComponent<Transform2DComponent>()->GetVertex(i);
		}

		float x = 1.0f / 9.0f;
		float y = 1.0f;
		float TexX = frameCount % 9 * x;
		float TexY = 0.0f;

		vertex[0].TexCoord = XMFLOAT2(TexX, TexY);
		vertex[1].TexCoord = XMFLOAT2(TexX + x, TexY);
		vertex[2].TexCoord = XMFLOAT2(TexX, TexY + y);
		vertex[3].TexCoord = XMFLOAT2(TexX + x, TexY + y);
		GetComponent<Transform2DComponent>()->SetVertex(vertex);

		frameCount++;
	}

	if (frameCount >= 10.0f) isDelete = true;

	DeleteRecest();

	for (auto com : ComponentList)
	{
		com->Update();
	}
}

void SlashSecound::Draw()
{

	for (auto com : ComponentList)
	{
		com->Draw();
	}

}

void SlashSecound::HitAttackCollision()
{
	XMFLOAT3 pos = GetComponent<Colider>()->GetPosition();

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
					m_ParentObject->SetHitStopState(true);
					Explosion* ex = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<Explosion>(1);
					//プレイヤーを登録
					ex->AddParentGameObject(m_ParentObject);

					ex->GetComponent<Transform>()->SetPosition(pos);
					ex->GetComponent<Transform>()->SetSize(XMFLOAT3(0.7f, 0.7f, 0.7f));

					Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
					//MPを回復させる
					States* sta = player->GetStates();

					float mp = sta->MP + 1.0f;
					if (mp > 50.0f)
					{
						mp = 50.0f;
					}

					player->SetStates(sta->HP, mp, sta->hit, sta->hitCount);
					/*float hp = obj->GetStates().HP;
					hp -= 1.0f;
					obj->SetStates(hp, 0.0f, true,count);*/
					int count = obj->GetStates()->hitCount;
					count++;

					obj->SetHitCount(count);
					if (count >= 2) obj->SetHit(true);
					m_HitObjectList.push_back(obj);

					XMFLOAT3 pos = GetComponent<Transform>()->GetPosition();
					XMFLOAT3 playerRot = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetRotation();

					pos.y += 2.0f;

					XMFLOAT3 anglePos = GetComponent<Colider>()->GetAddAnglePosition(playerRot, XMFLOAT3(1.0f, 0.0f, 0.0f));
					anglePos = { anglePos.x * 0.5f,anglePos.y * 0.5f ,anglePos.z * 0.5f };
					
					

					//HPを減らす
					float hp = obj->GetStates()->HP;
					hp -= m_AttackPoint;
					obj->SetHp(hp);

					int digits = std::log10(m_AttackPoint);

					digits++;

					//ダメージ表示を生成
					TextAttackPoint(digits);

					hit = true;

				}
			}

			if (hit == true) m_HitCount++;
			
		}
	}


}

void SlashSecound::TextAttackPoint(int n)
{

	//プレイヤー情報取得
	Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
	Transform* playerTrans = player->GetComponent<Transform>();
	XMFLOAT3 playerRot = playerTrans->GetRotation();
	XMFLOAT3 playerFoward = playerTrans->GetFoward();
	XMFLOAT3 playerRight = playerTrans->GetRight();

	float textToWiedth = 0.5f;


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
			textPos, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f));
		text[i]->SetTextUV(num);

		textPos = { textPos.x - (playerRight.x * textToWiedth),textPos.y /*- anglePos.y*/,textPos.z - (playerRight.z * textToWiedth) };

	}

	
}
