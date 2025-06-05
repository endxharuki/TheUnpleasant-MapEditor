#pragma once
#include <string>
#include <iostream>
#include <list>
#include "imgui.h"

class Component
{
private:

protected:

	
	int ComponentNumber = 0;
	std::string m_Tag = "";

	bool isDelete = false;

public:

	Component() = default;
	

	virtual ~Component() {};

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void ReInit(){}

	const int GetComponentNumber() const { return ComponentNumber; }
	const std::string GetComponentTag() const { return m_Tag; }

	bool DeleteRecest() {

		return isDelete;
	}
	void SetDelete(bool del)
	{
		isDelete = del;
	}

};

