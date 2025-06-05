#pragma once
#include "gameObjectComponent.h"

class Transform3DComponent;
class BoxColiderComponent;

class Enemy;


enum AIControllerState
{
	Idle,
	Move,
	Rotate,
	Attack,
	Died

};


class EnemyState : public GameObjectComponent
{

private:
	

protected:

	Enemy* parentEnemy;

public:

	virtual ~EnemyState() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Uninit() = 0;
	
	void AddParentEnemy(Enemy* enemy)
	{
		parentEnemy = enemy;
	}

};

class EnemyIdleState : public EnemyState
{
private:

public:

	virtual ~EnemyIdleState(){}
	void Init() override;
	void Update() override;
	void Uninit() override;

};
class EnemyMoveState : public EnemyState
{
private:

public:

	virtual ~EnemyMoveState() {}
	void Init() override;
	void Update() override;
	void Uninit() override;

	// Å’ZŒo˜H‚ÌŠp“x·‚ðŒvŽZ
	float ShortestAngleDifference(float current, float target) {
		float diff = target - current;
		while (diff > 3.141592 * 0.5f) diff -= 3.141592;
		while (diff < -3.141592 * 0.5f) diff += 3.141592;
		return diff;
	}
	// ŒÊ“x‚ð“x‚É•ÏŠ·
	float radiansToDegrees(float radians) {
		return radians * (180.0f / 3.14159265358979323846f);
	}

	// “x‚ðŒÊ“x‚É•ÏŠ·
	float degreesToRadians(float degrees) {
		return degrees * (3.14159265358979323846f / 180.0f);
	}




};

class EnemyRotateState : public EnemyState
{
private:

public:

	virtual ~EnemyRotateState() {}
	void Init() override;
	void Update() override;
	void Uninit() override;

	// Å’ZŒo˜H‚ÌŠp“x·‚ðŒvŽZ
	float ShortestAngleDifference(float current, float target) {
		float diff = target - current;
		while (diff > 3.141592 * 0.5f) diff -= 3.141592;
		while (diff < -3.141592 * 0.5f) diff += 3.141592;
		return diff;
	}
	// ŒÊ“x‚ð“x‚É•ÏŠ·
	float radiansToDegrees(float radians) {
		return radians * (180.0f / 3.14159265358979323846f);
	}

	// “x‚ðŒÊ“x‚É•ÏŠ·
	float degreesToRadians(float degrees) {
		return degrees * (3.14159265358979323846f / 180.0f);
	}

};

class EnemyAttackState : public EnemyState
{
private:

public:

	virtual ~EnemyAttackState() {}
	void Init() override;
	void Update() override;
	void Uninit() override;

	float endFrame = 100.0f;
	float attackFrame = 0.0f;


};

class EnemyDiedState : public EnemyState
{
private:

public:

	virtual ~EnemyDiedState() {}
	void Init() override;
	void Update() override;
	void Uninit() override;

	float endFrame = 100.0f;
	float downFrame = 0.0f;

	float alpah = 0.0f;


};

class EnemyAI : public GameObjectComponent
{
private:

	EnemyState* State;
	Enemy* m_Parent;

public:

	void Init();
	void Update();
	void Uninit();
	void ChangeState(AIControllerState state , Enemy* parent);
	void AddParent(Enemy* parent)
	{
		m_Parent = parent;
		State->AddParentEnemy(parent);
	}

	EnemyState* GetState()
	{
		return State;

	}

};




