#pragma once
#include <string>
#include <iostream>
#include <list>


class GameObject;

class Component
{
private:

protected:

	GameObject* m_GameObject = nullptr; //親のゲームオブジェクト
	bool isDelete = false;
	int ComponentNumber = 0;
	std::string m_Tag = "";

public:

	Component() = default;
	Component(GameObject* Object) : m_GameObject(Object){}

	virtual ~Component() {};

	virtual void Init() {}
	virtual void Uninit(){}
	virtual void Update(){}
	virtual void Draw (){}
	virtual void ReInit(){}

	const int GetComponentNumber() const { return ComponentNumber; }
	const std::string GetComponentTag() const { return m_Tag; }

	void AcceptGameObject(GameObject* obj) {

		m_GameObject = obj;
	 }
	GameObject* GetGameObject() {

		return m_GameObject;
	}
	bool DeleteRecest() {
		if (isDelete) {

			Uninit();
			delete this;
			return true;
		}
		else {
			return false;
		}
	}

};

