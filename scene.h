#pragma once

class SceneState;
class GameScene;

class Scene
{
private:

	
	explicit Scene(SceneState* pSceneState);
	Scene() = default;
	virtual ~Scene();
	SceneState* m_SceneState;

public:


	void InitSceneState(SceneState* pSceneState);
	void ChangeSceneState(SceneState* pSceneState);

	static Scene* GetInstance() {
		static Scene* scene = new Scene;
		return scene;
	}

	template <class T>
	T* GetScene()
	{
		T* s = static_cast<T*> (m_SceneState);
		if (s != nullptr) {
			return s;
		}
		return nullptr;
	}
	 
	SceneState* GetCurrentScene() const
	{
		return m_SceneState;
	}


	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	

};

