#pragma once
//ゲームシーンをインクルードしちゃ駄目よ
#include <string>
#include <list>
#include "sceneState.h"
#include <iostream>
#include <tuple>
#include <string>
#include "texture.h"
#include "texture2DComponent.h"
#include "gameObject.h"

class UI : public GameObject
{
private:
	SceneState* InScene = nullptr;

protected:

	std::list<UIComponent*> ComponentList; //このゲームオブジェクトにくっついているコンポーネント達
	bool isDelete = false;
	std::string Tag = "";

	ID3D11ShaderResourceView* m_Texture = NULL;

	float posX = 0.0f, posY = 0.0f;
	float sizeX = 200.0f, sizeY = 200.0f;

	bool OnDraw = true;

	int number = 0;

public:

	UI() {}
	~UI() {};
	bool DeleteRecest() {

		return isDelete;
	}
	void SetDelete(bool del)
	{
		isDelete = del;
	}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;

	void SetOnDraw(bool On) { OnDraw = On; }
	bool GetOnDraw() const { return OnDraw; }

	SceneState* GetScene() { return InScene; }
	void SetScene(SceneState* scene) { InScene = scene; }
	std::string GetTag() { return Tag; }

	void SetNumber(int num) { number = num; }
	int GetNumber() { return number; }

	std::list<UIComponent*> GetComponentList() { return ComponentList; }

	void ComponentListSort()
	{
		ComponentList.sort([](const UIComponent* a, const UIComponent* b)
			{
				return a->GetComponentNumber() < b->GetComponentNumber();
			}
		);
	}
	template <class T>
	T* AddComponent()
	{
		T* comp = new T;
		comp->AcceptGameObject(this);
		ComponentList.push_back(comp);

		return comp;

	}

	template <class T>
	T* GetComponent(int i)
	{
		for (auto com : ComponentList) {
			T* ret = dynamic_cast<T*>(com);
			if (ret != nullptr)
			{
				if (ret->GetComponentNumber() == i)
				{

					return ret;
				}
			}
		}
		return nullptr;
	}

	void SetVertexPosition(float posx, float posy, float sizex, float sizey)
	{
		posX = posx;
		posY = posy;
		sizeX = sizex;
		sizeY = sizey;
	}

	void SetPosTexNumber(float posx, float posy, float sizex, float sizey, ID3D11ShaderResourceView* texture, int num)
	{
		posX = posx;
		posY = posy;
		sizeX = sizex;
		sizeY = sizey;

		m_Texture = texture;

		number = num;
	}


};
