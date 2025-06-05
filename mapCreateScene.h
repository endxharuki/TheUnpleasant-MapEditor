#pragma once
#include "sceneState.h"
#include "gameObject.h"
#include "ui.h"
#include <list>
#include "imguiManager.h"
#include <fstream>
#include <map>
#include "editorCamera.h"


class MapCreateScene : public SceneState
{
private:

	std::list<GameObject*> m_GameObjectList[3];
	std::list<UI*> m_TextureList;

	bool isGoal = false;
	bool isClear = false;

	ImguiManager* imgui = nullptr;

	std::vector<std::string> availableObjectTypes = { "Block", "Tree", "Grass" };
	
	GameObject* m_EditObject = nullptr;

	std::map<std::string, GameObject*> m_GameObjectKindList;
	EditorCamera* m_Camera = nullptr;

	enum ObjectType
	{
		BLOC,
		NONE
	};

	float ndcX;
	float ndcY;

public:

	virtual ~MapCreateScene(){}

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

	void DrawImGui();


	void RegisterGameObject(const std::string name, GameObject* obj)
	{
		m_GameObjectKindList[name] = obj;

	}

	float GetNdcX()const { return ndcX; }
	float GetNdcY() const { return ndcY; }

	void SaveMap(const std::string& filename);
	void LoadMap(const std::string& filename);

	void CopyGameObject();
	void CopyInstallMeshField(XMFLOAT3 pos);

};

