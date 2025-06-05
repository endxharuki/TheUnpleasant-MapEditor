#include "main.h"
#include "playerState.h"
#include "playerIdleState.h"
#include "transform3DAnimationComponent.h"
#include "player.h"

void PlayerIdleState::Enter()
{
	m_ParentPlayer->GetComponent<Transform3DAnimationComponent>()->SetAnimationState("Idle");
}

void PlayerIdleState::Update()
{



}

void PlayerIdleState::Exit()
{
}
