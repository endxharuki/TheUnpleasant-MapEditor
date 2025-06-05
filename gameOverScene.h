#pragma once
#include "sceneState.h"
#include "gameObject.h"
#include "textrue2D.h"
#include <list>
#include "audio.h"

class TitleScene : public SceneState
{
private:

	std::list<GameObject*> m_GameObjectList[3];
	std::list<Texture2D*> m_TextureList;

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

	std::list<GameObject*> GetGameObjectList()
	{
		return m_GameObjectList[1];
	}

	Texture2D* GetTexture(int i)
	{
		for (auto tex : m_TextureList)
		{
			if (tex->GetNumber() == i)
			{
				return tex;
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
	template<class T>
	T* AddTexture()
	{
		T* tex = new T;
		m_TextureList.push_back(tex);
		tex->SetScene(this);
		return tex;

	}


};

