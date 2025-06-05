#pragma once
#include <string>
#include <iostream>
#include <list>
#include "component.h"

class UI;

class UIComponent : public Component
{
private:

protected:

	UI* m_UI = nullptr; //親のゲームオブジェクト
	bool isDelete = false;
	int ComponentNumber = 0;
	std::string m_Tag = "";

public:

	UIComponent() = default;
	UIComponent(UI* ui) : m_UI(ui){}

	virtual ~UIComponent() {};

	virtual void Init() override {}
	virtual void Uninit()override {}
	virtual void Update()override {}
	virtual void Draw ()override {}
	virtual void ReInit()override {}


	void SetComponentNumber(int i) { ComponentNumber = i; }
	const int GetComponentNumber()  const  { return ComponentNumber; }
	const std::string GetComponentTag() const  { return m_Tag; }

	void AcceptGameObject(UI* ui) {

		m_UI = ui;
	 }
	UI* GetGameObject() {

		return m_UI;
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

