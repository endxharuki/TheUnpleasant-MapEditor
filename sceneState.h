#pragma once


class Scene;

enum SceneName
{
	TITLE,
	GAME,
	RESULT,
	MAPCREATE,
	NONE
};

class SceneState
{
private:

protected:

	
	SceneName m_SceneName = NONE;
	
public:

	virtual ~SceneState(){}
	virtual void Init()  = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;

	
	

	SceneName GetSceneName() const
	{
		return m_SceneName;
	}

};

