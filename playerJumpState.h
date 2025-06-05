#pragma once
#include "playerState.h"

class PlayerJumpState : public PlayerState
{
private:
	XMFLOAT3 m_Vector = { 0.2f,0.5f,0.2f };
	bool isJump = false;
	int jumpFrame = 0;

public:
	~PlayerJumpState() = default;

	void Enter() override;
	void Update() override;
	void Exit() override;

	StateType GetName() const { return StateType::Jump; }

};