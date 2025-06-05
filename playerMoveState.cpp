#include "main.h"
#include "playerState.h"
#include "transform3DAnimationComponent.h"
#include "player.h"
#include "playerMoveState.h"
#include "capsuleColiderComponent.h"
#include "scene.h"
#include "gameScene.h"
#include "playerCamera.h"

void PlayerMoveState::Enter()
{
	m_ParentPlayer->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Run");
	m_Vector = { 0.2f,0.5f,0.2f };
}

void PlayerMoveState::Update()
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

	TransRot = XMFLOAT3(TransRot.x, posRot + cameraRot.y, TransRot.z);
	vel = XMFLOAT3(sinf(TransRot.y) * m_Vector.x, 0.0f, cosf(TransRot.y) * m_Vector.z);
	TransPos = XMFLOAT3(TransPos.x + vel.x, TransPos.y, TransPos.z + vel.z);

    col->SetPosition(TransPos);
	col->SetRotation(TransRot);
	col->SetVelocity(vel);
	//GetComponent<CapsuleColiderComponent>()->MoveCollision();

	trans->SetPosition(TransPos);
	trans->SetRotation(TransRot);
	trans->SetVelocity(vel);

}

void PlayerMoveState::Exit()
{
}
