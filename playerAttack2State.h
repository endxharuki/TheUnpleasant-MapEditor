#pragma once
#include "playerState.h"

class PlayerAttack2State : public PlayerState
{
private:
	//Å‰‚ÌUŒ‚‚ÌƒtƒŒ[ƒ€‚Æ^‹U
	float attackFrame = 0.0f;

public:
	~PlayerAttack2State() = default;

	void Enter() override;
	void Update() override;
	void Exit() override;

	StateType GetName() const { return StateType::Attack2; }

};