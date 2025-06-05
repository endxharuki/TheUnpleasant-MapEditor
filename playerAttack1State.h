#pragma once
#include "playerState.h"

class PlayerAttack1State : public PlayerState
{
private:
	//最初の攻撃のフレームと真偽
	float attackFrame = 0.0f;
	//次のアタックに移行するかどうか
	bool next = false;
	

public:
	~PlayerAttack1State() = default;

	void Enter() override;
	void Update() override;
	void Exit() override;

	StateType GetName() const { return StateType::Attack1; }

};

