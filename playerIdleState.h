#pragma once
#include "playerState.h"

class PlayerIdleState : public PlayerState
{

public:
	~PlayerIdleState() = default;

	void Enter() override;
	void Update() override;
	void Exit() override;

	StateType GetName() const { return StateType::Idle; }

};

