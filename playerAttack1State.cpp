#include "main.h"
#include "playerState.h"
#include "transform3DAnimationComponent.h"
#include "player.h"
#include "playerAttack1State.h"
#include "capsuleColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "playerCamera.h"
#include "input.h"
#include "slashFirst.h"

void PlayerAttack1State::Enter()
{
	m_ParentPlayer->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Slash1");
	//攻撃が終わるまでほかの状態への移行はできない
	m_ChangePossible = false;
	attackFrame = 50.0f;
	next = false;
}

void PlayerAttack1State::Update()
{

	attackFrame--;
	
	//プレイヤーを登録
	if (attackFrame == 40.0f) Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<SlashFirst>(1)->AddParentGameObject(m_ParentPlayer);
	//受付時間の処理
	if (attackFrame <= 20.0f)
	{
		if (Input::GetInstance()->GetKeyTrigger(VK_SPACE))
		{
			next = true;
		}
	}

	
	//終了
	if (attackFrame <= 0.0f)
	{
		m_ChangePossible = true;
		if (next == true)
		{
			m_ParentPlayer->ChangeState(StateType::Attack2);

		}
	}
	

}

void PlayerAttack1State::Exit()
{
}
