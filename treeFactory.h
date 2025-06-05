#pragma once
#include "tree.h"

#define POS_X (20)
#define POS_Z (20)

class TreeFactory : public GameObject
{
private:

	GameScene* m_ParentScene = nullptr;
	std::list<Tree*> m_TreeList;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void CreateTree();

	void SetGameScene(GameScene* parent) { m_ParentScene = parent; }
	std::list<Tree*> GetTreeList() { return m_TreeList; }


};

