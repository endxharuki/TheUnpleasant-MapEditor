#pragma once
#include "playerState.h"

class PlayerSkill1State : public PlayerState
{
private:
	
	//マジックblade用
	MagicBlade* m_Blade = nullptr;
	bool NoMagic = false;
	bool baladeOnce = false;
	int magicEndFrame = 0;
	float m_RotY = 0.0f;
	

public:
	~PlayerSkill1State() = default;

	void Enter() override;
	void Update() override;
	void Exit() override;

	StateType GetName() const { return StateType::Attack1; }

};

