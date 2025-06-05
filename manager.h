#pragma once

#include "polygon2D.h"
#include "gameObject.h"
#include <list>
#include "scene.h"
#include "sceneState.h"

class Manager
{
private:

	static Manager* manager;
	/*std::list<GameObject*> m_GameObjectList;*/

public:

	static Manager* GetInstance() {
		static Manager manager;
		return &manager;
	}
	static Manager* GetInstanceC() {
		if (!manager) {
			manager = new Manager;
		}
		return manager;
	}
	/*static*/ void Init();
	/*static*/ void Uninit();
	/*static*/ void Update();
	/*static*/ void Draw();

	/*void AddGameObject (GameObject* obj){

		m_GameObjectList.push_back(obj);

	}
	std::list<GameObject*> GetGameObject() const {

		return m_GameObjectList;
	}*/

};