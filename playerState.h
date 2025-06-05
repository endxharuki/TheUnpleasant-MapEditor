#pragma once
#include "gameObjectComponent.h"

//プレイヤーのステート一覧
enum class StateType 
{
	Idle,
	Move,
	Jump,
	Roll,
	Attack1,
	Attack2,
	Skill1

};

class Player;

class PlayerState
{
protected:

	Player* m_ParentPlayer = nullptr;
	bool m_ChangePossible = true;

public:
	virtual ~PlayerState() = default;

	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual StateType GetName() const = 0;

	void AddParentPlayer(Player* player) { m_ParentPlayer = player; }
	void SetChangePossible(bool state) { m_ChangePossible = state; }
	bool GetChangePossible() const { return m_ChangePossible; }
	

};

