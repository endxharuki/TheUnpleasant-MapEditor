#include "main.h"
#include "playerState.h"
#include "transform3DAnimationComponent.h"
#include "player.h"
#include "playerJumpState.h"
#include "capsuleColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "playerCamera.h"

void PlayerJumpState::Enter()
{
	m_ParentPlayer->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");
	m_Vector = { 0.2f,0.5f,0.2f };
	isJump = true;
	jumpFrame = 0;
	//ジャンプが終わるまでほかの状態への移行はできない
	m_ChangePossible = false;
}

void PlayerJumpState::Update()
{
	Transform* trans = m_ParentPlayer->GetComponent<Transform>();
	Colider* col = m_ParentPlayer->GetComponent<Colider>();

	XMFLOAT3 TransPos = trans->GetPosition();
	XMFLOAT3 TransRot = trans->GetRotation();
	XMFLOAT3 ColPos = col->GetPosition();
	XMFLOAT3 ColRot = col->GetRotation();
	XMFLOAT3 vel = trans->GetVelocity();

	float posRot = trans->GetRot();

	PlayerCamera* camera = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<PlayerCamera>();
	XMFLOAT3 cameraRot = camera->GetComponent<Transform>()->GetRotation();
	float PI = 3.141592;

	if (GetKeyState('S') & 0x8000) {
		posRot = 0.0f;
		/*m_AnimationName = "Run";*/
	}
	if (GetKeyState('W') & 0x8000) {

		posRot = PI;
		/*m_AnimationName = "Run";*/
	}
	if (GetKeyState('D') & 0x8000) {

		posRot = -PI / 2;
		/*m_AnimationName = "Run";*/
	}
	if (GetKeyState('A') & 0x8000) {

		posRot = PI / 2;
		/*m_AnimationName = "Run";*/
	}
	if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || GetKeyState('A') & 0x8000 || GetKeyState('D') & 0x8000)
	{
		TransRot = XMFLOAT3(TransRot.x, posRot + cameraRot.y, TransRot.z);
		vel = XMFLOAT3(sinf(TransRot.y) * m_Vector.x, 0.0f, cosf(TransRot.y) * m_Vector.z);
		TransPos = XMFLOAT3(TransPos.x + vel.x, TransPos.y, TransPos.z + vel.z);
		m_ParentPlayer->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Run");

	}
	 
	jumpFrame++;
	if (jumpFrame <= 10) TransPos.y += (m_Vector.y * 0.5f);
	else if (jumpFrame <= 15) TransPos.y += (m_Vector.y * 0.35f);
	else if (jumpFrame <= 20)TransPos.y += (m_Vector.y * 0.25f);
	else if (jumpFrame <= 25)TransPos.y += (m_Vector.y * 0.15f);
	else if (jumpFrame <= 30)TransPos.y += (m_Vector.y * 0.05f);
	else {
		//isJump = false;
		m_ChangePossible = true;
	}
	

	col->SetPosition(TransPos);
	col->SetRotation(TransRot);
	col->SetVelocity(vel);
	//GetComponent<CapsuleColiderComponent>()->MoveCollision();

	trans->SetPosition(TransPos);
	trans->SetRotation(TransRot);
	trans->SetVelocity(vel);

}

void PlayerJumpState::Exit()
{
}
