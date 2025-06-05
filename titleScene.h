#pragma once
#include "sceneState.h"
#include "gameObject.h"
#include "textrue2D.h"
#include <list>
#include "audio.h"
#include "ui.h"

class TitleScene : public SceneState
{
private:

	std::list<GameObject*> m_GameObjectList[3];
	std::list<UI*> m_TextureList;

	float drawFrame = 0.0f;

	Audio* audio = nullptr;

public:

	virtual ~TitleScene(){}

	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void DeleteGameObject()
	{
		for (int i = 0;i < 3;i++) {
			m_GameObjectList[i].remove_if([](GameObject* obj) {

				if (obj->DeleteRecest() == true) {

					delete obj;
					return true;
				}

				return false;
				}
			);
		}
	}

	std::list<GameObject*> GetGameObjectList(int i)
	{
		return m_GameObjectList[i];
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
			for (GameObject* obj : m_GameObjectList[i]) {
				T* ret = dynamic_cast<T*> (obj);
				if (ret != nullptr) {

					return ret;
				}
			}
		}
		return nullptr;
	}
};

