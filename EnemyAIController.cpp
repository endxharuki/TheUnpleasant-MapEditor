#include "main.h"
#include "EnemyAIController.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "sceneState.h"
#include "enemy.h"
#include "player.h"
#include "meshField.h"
#include "transform3DAnimationComponent.h"
#include "enemySlash.h"
#include "capsuleColiderComponent.h"

//EnemyStateBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Idle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EnemyIdleState::Init()
{



}

void EnemyIdleState::Update()
{

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 enemyPos = parentEnemy->GetComponent<Transform>()->GetPosition();

	float diffrenceX = playerPos.x - enemyPos.x;
	float diffrenceZ = playerPos.z - enemyPos.z;

	float len = sqrt(diffrenceX * diffrenceX + diffrenceZ * diffrenceZ);

	parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");

	//プレイヤーまでのベクトル
	XMFLOAT3 vector = { playerPos.x - enemyPos.x,0.0f,playerPos.z - enemyPos.z };
	vector = Normalize(vector);
	XMFLOAT3 FrontVector = parentEnemy->GetComponent<Transform>()->GetFoward();
	XMFLOAT3 RightVector = parentEnemy->GetComponent<Transform>()->GetRight();

	float PI = 3.141592;

	//プレイヤーまでのベクトルとエネミーが向いている方向のベクトルのなす角度を求める
	{
		// 内積を計算
		float dotProduct = Dot(vector, FrontVector);

		// ベクトルの大きさを計算
		float vec1len = Length(vector);
		float vec2len = Length(FrontVector);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cosθを計算
		float cosTheta = dotProduct / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acos(cosTheta);
		float angleDegrees = angleRadians * 180.0f / PI;

		//ほぼ正面を向いている
		if (angleDegrees < 5.0f)
		{
			//近かったら攻撃する
			/*if (len <= 5.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Attack, parentEnemy);
			}
			else if(len <= 50.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Move, parentEnemy);

			}*/
			return;
		}
		else
		{
			//parentEnemy->GetComponent<EnemyAI>()->ChangeState(Rotate, parentEnemy);
		}

		//parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");
	}


	//hpがなくなったら死ぬモーションへ移行
	if (parentEnemy->GetStates()->HP <= 0.0f)
	{
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Died, parentEnemy);
	}

}

void EnemyIdleState::Uninit()
{
}
//MoveState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EnemyMoveState::Init()
{



}

void EnemyMoveState::Update()
{
	//アニメーションの変更
	parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");

	Transform* transform = parentEnemy->GetComponent<Transform>();

	//ゲームシーンを取得
	GameScene* scene = Scene::GetInstance()->GetScene<GameScene>();
	XMFLOAT3 playerPos = scene->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();

	XMFLOAT3 enemyPos = transform->GetPosition();
	XMFLOAT3 rotation = transform->GetRotation();

	float diffrenceX = playerPos.x - enemyPos.x;
	float diffrenceZ = playerPos.z - enemyPos.z;

	float len = sqrt(diffrenceX * diffrenceX + diffrenceZ * diffrenceZ);

	//ベクトルの向き
	//XMFLOAT3 vector = {  playerPos.x - enemyPos.x,playerPos.y - enemyPos.y,playerPos.z - enemyPos.z};
	XMFLOAT3 vector = { playerPos.x - enemyPos.x,0.0f,playerPos.z - enemyPos.z };

	float length = Length(vector);
	
	//プレイヤーまでのベクトル
	vector = Normalize(vector);
	XMFLOAT3 FrontVector = parentEnemy->GetComponent<Transform>()->GetFoward();
	XMFLOAT3 RightVector = parentEnemy->GetComponent<Transform>()->GetRight();

	float PI = 3.141592;

	//プレイヤーまでのベクトルとエネミーが向いている方向のベクトルのなす角度を求める
	{
		// 内積を計算
		float dotProduct = Dot(vector, FrontVector);

		// ベクトルの大きさを計算
		float vec1len = Length(vector);
		float vec2len = Length(FrontVector);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cosθを計算
		float cosTheta = dotProduct / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acos(cosTheta);
		float angleDegrees = angleRadians * 180.0f / PI;

		//ほぼ正面を向いている
		if (angleDegrees < 5.0f)
		{
			//近かったら攻撃する
			if (len <= 5.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Attack, parentEnemy);
				return;
			}
			//移動させる
			{
				float speed = 0.02f;
				XMFLOAT3 moveVector = { vector.x * speed,vector.y * speed,vector.z * speed };

				enemyPos = { enemyPos.x + moveVector.x,enemyPos.y + moveVector.y ,enemyPos.z + moveVector.z };

				//高さをメッシュフィールドに合わせる
				float higth = scene->GetGameObject<MeshField>()->GetHeight(enemyPos);
				enemyPos.y = higth;
			}

			transform->SetPosition(enemyPos);
			transform->SetRotation(rotation);


			return;
		}
		else
		{
			parentEnemy->GetComponent<EnemyAI>()->ChangeState(Rotate, parentEnemy);
		}
	}

	

	//hpがなくなったら死ぬモーションへ移行
	if (parentEnemy->GetStates()->HP <= 0.0f)
	{
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Died, parentEnemy);
	}
	//プレイヤーにある程度近くなったら攻撃する
	if (length <= 5.0f) parentEnemy->GetComponent<EnemyAI>()->ChangeState(Attack, parentEnemy);

}

void EnemyMoveState::Uninit()
{
}
//Rotate
////
////
void EnemyRotateState::Init()
{

}

void EnemyRotateState::Update()
{

	//アニメーションの変更
	parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 enemyPos = parentEnemy->GetComponent<Transform>()->GetPosition();

	float diffrenceX = playerPos.x - enemyPos.x;
	float diffrenceZ = playerPos.z - enemyPos.z;

	float len = sqrt(diffrenceX * diffrenceX + diffrenceZ * diffrenceZ);

	//プレイヤーまでのベクトル
	XMFLOAT3 vector = { playerPos.x - enemyPos.x,0.0f,playerPos.z - enemyPos.z };
	vector = Normalize(vector);
	XMFLOAT3 FrontVector = parentEnemy->GetComponent<Transform>()->GetFoward();
	XMFLOAT3 RightVector = parentEnemy->GetComponent<Transform>()->GetRight();

	float PI = 3.141592;

	//プレイヤーまでのベクトルとエネミーが向いている方向のベクトルのなす角度を求める
	{
		// 内積を計算
		float dotProduct = Dot(vector, FrontVector);

		// ベクトルの大きさを計算
		float vec1len = Length(vector);
		float vec2len = Length(FrontVector);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cosθを計算
		float cosTheta = dotProduct / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acos(cosTheta);
		float angleDegrees = angleRadians * 180.0f / PI;

		//ほぼ正面を向いている
		if (angleDegrees < 5.0f)
		{
			//近かったら攻撃する
			if (len <= 5.0f)
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Attack, parentEnemy);
			}
			else
			{
				parentEnemy->GetComponent<EnemyAI>()->ChangeState(Move, parentEnemy);

			}
			return;
		}

		//parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Walk");
	}

	//プレイヤーまでのベクトルとエネミーが向いている方向の右側のベクトルのなす角度を求める
	{
		// 内積を計算
		float dotProduct = Dot(vector, RightVector);

		// ベクトルの大きさを計算
		float vec1len = Length(vector);
		float vec2len = Length(RightVector);

		//ベクトルの大きさが0だったら処理を中止
		if (vec1len == 0 || vec2len == 0)
		{
			return;
		}

		// cosθを計算
		float cosTheta = dotProduct / (vec1len * vec2len);

		cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);

		float angleRadians = acos(cosTheta);
		float angleDegrees = angleRadians * 180.0f / PI;


		if (angleDegrees < 90.0f)
		{
			XMFLOAT3 rot = parentEnemy->GetComponent<Transform>()->GetRotation();
			rot.y += 0.02f;
			parentEnemy->GetComponent<Transform>()->SetRotation(rot);
		}
		else
		{
			XMFLOAT3 rot = parentEnemy->GetComponent<Transform>()->GetRotation();
			rot.y -= 0.02f;
			parentEnemy->GetComponent<Transform>()->SetRotation(rot);
		}


	}
	
	//hpがなくなったら死ぬモーションへ移行
	if (parentEnemy->GetStates()->HP <= 0.0f)
	{
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Died, parentEnemy);
	}

}

void EnemyRotateState::Uninit()
{
}

//Attack
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EnemyAttackState::Init()
{
}

void EnemyAttackState::Update()
{
	XMFLOAT3 pos = parentEnemy->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 rot = parentEnemy->GetComponent<Transform>()->GetRotation();

	

	if (attackFrame == 1.0f)
	{
		parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Attack");
	}
	if (attackFrame == 40.0f) {
		EnemySlash* slash = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<EnemySlash>(2);
		slash->SetEnemyPos(pos);
		slash->SetEnemyRot(rot);
		slash->OnInit();
		slash->Init();

	}
	if (attackFrame == 200)
	{
		
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Idle, parentEnemy);
	}
	if (attackFrame == 220) attackFrame = 0.0f;
	attackFrame++;

	//hpがなくなったら死ぬモーションへ移行
	if (parentEnemy->GetStates()->HP <= 0.0f)
	{
		parentEnemy->GetComponent<EnemyAI>()->ChangeState(Died, parentEnemy);
	}

	/*if(attackFrame >= endFrame) parentEnemy->GetComponent<EnemyAI>()->ChangeState(Idle, parentEnemy);*/
	
}

void EnemyAttackState::Uninit()
{
}
//Died
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyDiedState::Init()
{
}

void EnemyDiedState::Update()
{
	Colider* com = parentEnemy->GetComponent<CapsuleColiderComponent>();
	parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Died");
	if (downFrame == 0.0f)
	{
		com->SetDelete(true);
	}
	if (downFrame >= 300.0f)
	{
		
		//死んだモーションの跡は消す
		/*parentEnemy->SetDelete(true);*/
		alpah += 0.005f;
		parentEnemy->GetComponent<Transform3DAnimationComponent>()->GetModel()->SetAlpha(alpah);
		parentEnemy->GetComponent<Transform3DAnimationComponent>()->SetDelay(1000);
		if (alpah >= 1.0f)
		{
			parentEnemy->SetDelete(true);
		}
	}

	downFrame++;
}

void EnemyDiedState::Uninit()
{
}

//EnemyAI
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EnemyAI::Init()
{
	EnemyIdleState* state = new EnemyIdleState;
	State = state;
	State->Init();

	State;
}

void EnemyAI::Update()
{

	State->AddParentEnemy(m_Parent);
	State->Update();

}

void EnemyAI::Uninit()
{

	State->Uninit();
	delete State;
}

void EnemyAI::ChangeState(AIControllerState state, Enemy* parent)
{

	if (state == Idle)
	{
		EnemyIdleState* state = new EnemyIdleState;
		State = state;
		State->AddParentEnemy(m_Parent);
		
	}
	if (state == Move)
	{
		EnemyMoveState* state = new EnemyMoveState;
		State = state;
		State->AddParentEnemy(m_Parent);

	}
	if (state == Rotate)
	{
		EnemyRotateState* state = new EnemyRotateState;
		State = state;
		State->AddParentEnemy(m_Parent);
	}
	if (state == Attack)
	{
		EnemyAttackState* state = new EnemyAttackState;
		State = state;
		State->AddParentEnemy(m_Parent);
	}
	if (state == Died)
	{
		EnemyDiedState* state = new EnemyDiedState;
		State = state;
		State->AddParentEnemy(m_Parent);
	}
}


