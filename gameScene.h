#pragma once
#include "sceneState.h"
#include "gameObject.h"
#include "ui.h"
#include <list>
#include "imguiManager.h"

class GameScene : public SceneState
{
private:

	std::list<GameObject*> m_GameObjectList[3];
	std::list<UI*> m_TextureList;

	bool isGoal = false;
	bool isClear = false;

	ImguiManager* imgui = nullptr;

public:

	virtual ~GameScene(){}

	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void DeleteGameObject();

	void SetClear(bool set) { isClear = set; }

	ImguiManager* GetImguiManager()
	{
		return imgui;
	}

	std::list<GameObject*> GetGameObjectList(int i)
	{
		return m_GameObjectList[i];
	}

	template<class T>
	T* AddGameObject(int layer)
	{
		T* obj = new T;
		m_GameObjectList[layer].push_back(obj);
		obj->SetScene(this);
		obj->Init();
		return obj;

	}
	template<class T>
	T* GetGameObject()
	{
		for (int i = 0;i < 3; i++) {
			for (auto obj : m_GameObjectList[i]) {
				T* ret = dynamic_cast<T*> (obj);
				if (ret != nullptr) {

					return ret;
				}
			}
		}
		return nullptr;
	}

	template<class T>
	T* AddUIObject()
	{
		T* obj = new T;
		m_TextureList.push_back(obj);
		obj->SetScene(this);

		return obj;

	}
	template<class T>
	T* GetUIObject(int i)
	{

		for (UI* obj : m_TextureList) {
			T* ret = dynamic_cast<T*> (obj);
			if (ret != nullptr) {
				if (ret->GetNumber() == i)
				{

					return ret;
				}
			}
		}
		return nullptr;
	}

};

