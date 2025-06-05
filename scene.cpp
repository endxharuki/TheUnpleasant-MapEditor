#include "main.h"
#include "scene.h"
#include "sceneState.h"


Scene::Scene(SceneState* pSceneState)
	: m_SceneState(pSceneState)
{

}

Scene::~Scene()
{
	delete m_SceneState;

}

void Scene::InitSceneState(SceneState* pSceneState)
{
	m_SceneState = pSceneState;
	Init();

}

void Scene::ChangeSceneState(SceneState* pSceneState)
{
	
	delete m_SceneState;
	m_SceneState = pSceneState;
	Init();

}

void Scene::Init()
{
	m_SceneState->Init();

}

void Scene::Uninit()
{
	m_SceneState->Uninit();

}

void Scene::Update()
{

	m_SceneState->Update();

}

void Scene::Draw()
{
	m_SceneState->Draw();

}
