#include "main.h"
#include "playerState.h"
#include "transform3DAnimationComponent.h"
#include "player.h"
#include "playerSkill1State.h"
#include "capsuleColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "playerCamera.h"
#include "input.h"
#include "slashFirst.h"
#include "circle2DColider.h"

void PlayerSkill1State::Enter()
{
	m_ParentPlayer->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");
	magicEndFrame = 0.0f;
	//マジックbladeを生成
	float mp = m_ParentPlayer->GetStates()->MP;

	if (mp >= 10.0f)
	{
		m_Blade = Scene::GetInstance()->GetScene<GameScene>()->AddGameObject<MagicBlade>(1);
		m_Blade->AddParentGameObject(m_ParentPlayer);
	}
	m_ChangePossible = false;

}

void PlayerSkill1State::Update()
{
	Transform* trans = m_ParentPlayer->GetComponent<Transform>();
	Colider* col = m_ParentPlayer->GetComponent<Colider>();

	XMFLOAT3 TransRot = trans->GetRotation();

	PlayerCamera* camera = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<PlayerCamera>();
	XMFLOAT3 cameraRot = camera->GetComponent<Transform>()->GetRotation();
	float PI = 3.141592;

	TransRot = XMFLOAT3(TransRot.x, PI + cameraRot.y, TransRot.z);

	col->SetRotation(TransRot);
	trans->SetRotation(TransRot);


	if (NoMagic == true)
	{
		if (magicEndFrame >= 50.0f)
		{
			m_ChangePossible = true;
			NoMagic = false;
		}
		magicEndFrame++;

		return;
	}

	//エイム処理
	float mp = m_ParentPlayer->GetStates()->MP;

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{

	}
	else
	{
		m_ChangePossible = true;
	}

	if (mp >= 10.0f)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<PlayerCamera>()->GetComponent<Circle2DColider>()->GetIsInObject() == true)
			{
				m_Blade->SetBurstState(true);
				m_ParentPlayer->SetMp(mp - 10.0f);
				NoMagic = true;
				m_ChangePossible = false;
			}
		}
	}

}

void PlayerSkill1State::Exit()
{	if (NoMagic == true) return;
	m_Blade->SetDelete(true);
	
}
