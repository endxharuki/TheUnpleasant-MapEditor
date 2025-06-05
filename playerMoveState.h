#pragma once
#include "playerState.h"

class PlayerMoveState : public PlayerState
{
private:
	XMFLOAT3 m_Vector = { 0.2f,0.5f,0.2f };

public:
	~PlayerMoveState() = default;

	void Enter() override;
	void Update() override;
	void Exit() override;

	StateType GetName() const { return StateType::Move; }

};