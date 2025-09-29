#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "playerCamera.h"
#include "field.h"
#include "player.h"
#include "mapCreateScene.h"
#include "resultScene.h"
#include "scene.h"
#include "enemy.h"
#include "block.h"
#include "explosion.h"
#include "transform3DComponent.h"
#include "transform2DComponent.h"
#include "score.h"
#include "timeLimit.h"
#include "sky.h"
#include "goal.h"
#include "resultScene.h"
#include "meshField.h"
#include "house.h"
#include "treeFactory.h"
#include "tree.h"
#include "fade.h"
#include "texturePool.h"
#include "input.h"
#include "shadowVolume.h"
#include "gameSky.h"
#include "magicBlade.h"
#include "textHp.h"
#include "textGauge.h"
#include "editorCamera.h"
#include "tree.h"
#include "grass.h"
//#include "slashFirst.h"


void MapCreateScene::Init()
{

	m_SceneName = MAPCREATE;
	Renderer::Init();
	//レイヤー順番バカ大事だから気を付けて
	TexturePool::GetInstance()->LoadTextrue();
	TexturePool::GetInstance()->Init();
	Input::GetInstance()->Init();

	m_Camera = AddGameObject<EditorCamera>(0);//2Dのゲームオブジェクトを入れたらカメラは一番最後


	AddGameObject<GameSky>(1);
	AddGameObject<MeshField>(1);

	AddGameObject<Block>(1);
	AddGameObject<Tree>(1)->GetComponent<Transform>()->SetPosition(XMFLOAT3(20.0f,2.0f,20.0f));
	//AddGameObject<Grass>(1)->GetComponent<Transform>()->SetPosition(XMFLOAT3(10.0f, 2.0f, 20.0f));


	for (auto tex : m_TextureList) {
		tex->Init();
	}
	
	imgui = new ImguiManager;
	imgui->Init();
}

void MapCreateScene::Uninit()
{
	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Uninit();
			delete obj;
		}
		if (i == 1)
		{
			for (auto tex : m_TextureList)
			{
				tex->Uninit();
			}
		}
		m_GameObjectList[i].clear();
	}
	TexturePool::GetInstance()->UnLoadTexture();
	imgui->Uninit();
	Input::GetInstance()->Uninit();
	
	Renderer::Uninit();
}

void MapCreateScene::Update()
{
	Input::GetInstance()->Update();

	for (int i = 0;i < 3;i++) {
		for (auto obj : m_GameObjectList[i]) {

			obj->Update();
		}
		if (i == 1)
		{
			for (auto tex : m_TextureList)
			{
				tex->Update();
			}
		}
	}

	DeleteGameObject();

	
	if (isGoal == true)
	{
		AddGameObject<Goal>(1)->GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 20.0f, 220.0f));
	}
	if (isClear == true)
	{
		Uninit();
		Scene::GetInstance()->ChangeSceneState(new ResultScene);
	}
	/*if (GetGameObject<Player>()->GetScore() >= 3) 
	{
		Uninit();
		Scene::GetInstance()->ChangeSceneState(new ResultScene);
	}*/
}

void MapCreateScene::Draw()
{

	ID3D11RasterizerState* wireframeState = nullptr;

	D3D11_RASTERIZER_DESC framerasterDesc = {};
	framerasterDesc.FillMode = D3D11_FILL_SOLID; // 通常モード
	framerasterDesc.CullMode = D3D11_CULL_BACK;      // カリングなし（両面描画）
	framerasterDesc.DepthClipEnable = TRUE;

	Renderer::GetDevice()->CreateRasterizerState(&framerasterDesc, &wireframeState);
	Renderer::GetDeviceContext()->RSSetState(wireframeState); // ラスタライザーステートを設定

	Renderer::Begin();
	
	imgui->ImGuiRendererInit();


	for (int i = 0; i < 3; i++) {
		for (auto obj : m_GameObjectList[i])
		{
			obj->Draw();
		}
		if (i == 1)
		{
			for (auto tex : m_TextureList)
			{
				tex->Draw();
			}
		}
	}

	Renderer::Update();


	DrawImGui();

	imgui->ImGuiRenderer();

	Renderer::End();

}

void MapCreateScene::DeleteGameObject()
{

	bool isDelete = false;
	for (int i = 0;i < 3;i++) {
		m_GameObjectList[i].remove_if([&isDelete](GameObject* obj) {

			if (obj->DeleteRecest() == true) {
			  //if(obj->GetTag() == "Enemy") Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->SetScore(1);
				obj->Uninit();
				delete obj;
				isDelete = true;
				return true;
			}

			return false;
			}
		);
	}
	if (isDelete == true)
	{
		m_EditObject = nullptr;
	}
	
	m_TextureList.remove_if([](UI* obj) {

		if (obj->DeleteRecest() == true)
		{
			obj->Uninit();
			delete obj;

			return true;
		}
		return false;
		}
	);
}

void MapCreateScene::DrawImGui()
{

	// Define UI layout sizes
	ImVec2 leftPanelSize = ImVec2(200, 450);
	ImVec2 bottomPanelSize = ImVec2(800, 200);
	ImVec2 rightPanelSize = ImVec2(200, 450);
	ImVec2 viewportSize = ImVec2(880, 550);
	
	size_t i = 0;
	ImGui::SetNextWindowPos(ImVec2(0, 100));
	ImGui::SetNextWindowSize(leftPanelSize);

	ImGui::Begin("Object List", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Text("Object");
	static int selectedObjectType = 0; // Index for the selected object type
	//if (ImGui::BeginCombo("Object Type", availableObjectTypes[selectedObjectType].c_str())) {
	for (auto obj : m_GameObjectList[1]) {

		ImGui::PushID(static_cast<int>(i));
		bool isSelected = (selectedObjectType == i);
		if (ImGui::Selectable(obj->GetTag().c_str(), isSelected)) {
			selectedObjectType = i;
			m_EditObject = obj;
		}
		ImGui::PopID();
		i++;
	}
	
	
	ImGui::End();

	// Bottom panel: Object types
	ImGui::SetNextWindowPos(ImVec2(0, leftPanelSize.y + 100));
	ImGui::SetNextWindowSize(ImVec2(bottomPanelSize.x + 500,bottomPanelSize.y));

	ImGui::Begin("Add Object", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Text("Add an object:");
	for (const auto& type : availableObjectTypes) {
		if (ImGui::Button(type.c_str(), ImVec2(100, 50))) {

			if (type == "Block")
			{
				AddGameObject<Block>(1)->GetComponent<Transform>()->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
			}
			if (type == "Tree")
			{
				AddGameObject<Tree>(1)->GetComponent<Transform>()->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
			}
			
		}
		ImGui::SameLine();
	}

	if (ImGui::Button("Save Map"))
	{
		SaveMap("MapData1.csv");
	}
	if (ImGui::Button("Load Map"))
	{

	}

	ImGui::End();
	// Right panel: Selected object details
	// Right panel : Selected object details
	ImGui::SetNextWindowPos(ImVec2(leftPanelSize.x + viewportSize.x, 100));
	ImGui::SetNextWindowSize(rightPanelSize);
	ImGui::Begin("Object Details", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Text("ObjectEdit");

	if (m_EditObject != nullptr)
	{
		for (auto obj : m_GameObjectList[1])
		{
			obj->SetEditMode(false);

		}
		Transform* trans = m_EditObject->GetComponent<Transform>();
		m_EditObject->SetEditMode(true);
		if (trans != nullptr)
		{
			
			//Transform* trans = m_EditObject->GetComponent<Transform>();
			XMFLOAT3 position = trans->GetPosition();
			XMFLOAT3 size = trans->GetSize();
			XMFLOAT3 rotation = trans->GetRotation();
			static float pos[3];
			static float scale[4];
			static float rot[3];
			pos[0] = position.x; pos[1] = position.y; pos[2] = position.z;
			scale[0] = size.x; scale[1] = size.y; scale[2] = size.z; scale[3] = 0.0f;
			rot[0] = rotation.x; rot[1] = rotation.y; rot[2] = rotation.z;

			ImGui::Text("Type: %s", m_EditObject->GetTag().c_str());
			ImGui::DragFloat3("Position", pos, 0.1f);
			ImGui::DragFloat4("Scale", scale, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat3("Rotation", rot, 0.1f, 0.0f, 3.141592f);

			for (int i = 0; i < 3; i++)
			{
				scale[i] += scale[3];
			}

			trans->SetPosition(XMFLOAT3(pos[0], pos[1], pos[2]));
			trans->SetSize(XMFLOAT3(scale[0], scale[1], scale[2]));
			trans->SetRotation(XMFLOAT3(rot[0], rot[1], rot[2]));
			

			if (ImGui::Button("Copy"))
			{
				CopyGameObject();
			}
			
			//メッシュフィールド上にオブジェクトをコピーする←量産しやすい
			static bool state;
			ImGui::Checkbox("CopyInstallMeshField", &state);
			if (state == true)
			{
				MeshField* m = nullptr;
				for (auto obj : m_GameObjectList[1])
				{
					if (obj->GetTag() == "MeshField") m = static_cast<MeshField*>(obj);
				}
				if (m != nullptr)
				{
					std::pair<bool, XMFLOAT3> data;
					m->RayCastFieldHitPosition(data);
					if (data.first == true)
					{
						CopyInstallMeshField(data.second);
					}

				}
			}
			

			if (ImGui::Button("Delete Object")) 
			{
				m_EditObject->SetDelete(true);
			}
		}

		m_EditObject->DrawImGui();
	}

	ImGui::End();

	//カメラオプション
	

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0));
	ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f));
	ImGui::Begin("Camera Option", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Text("Camera");

	GetGameObject<EditorCamera>()->DrawImGui();

	ImGui::End();

	//3D Viewport: Render area
	ImGui::SetNextWindowPos(ImVec2(leftPanelSize.x, 0));
	ImGui::SetNextWindowSize(viewportSize);
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("[3D Viewport Placeholder]");
	// Here you would render your 3D scene using your graphics API (e.g., DirectX)
	//ID3D11ShaderResourceView* pTextureSRV = TexturePool::GetInstance()->GetTexture("TitleBack");
	ID3D11ShaderResourceView* pTextureSRV = Renderer::GetTextureSRV();
	
	ImGui::Image(reinterpret_cast<ImTextureID>(pTextureSRV), ImVec2(viewportSize.x, viewportSize.y));

	if (ImGui::IsItemHovered())
	{
		m_Camera->SetCursorState(false);
	}
	else
	{
		m_Camera->SetCursorState(true);
	}
	
	// ゲーム画面のウィンドウ内のマウス座標を取得
	ImVec2 viewportPos = ImGui::GetCursorScreenPos();  // ImGui::Image() の左上座標
	ImVec2 mousePos = ImGui::GetMousePos();            // 現在のマウス座標
	
	//今のウィンドウ情報
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	Renderer::GetDeviceContext()->RSGetViewports(&numViewports, &viewport);


	//マウスのスクリーン座標を取得
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(GetActiveWindow(), &cursorPos);

	float cPosX = static_cast<float>(cursorPos.x);
	float cPosY = static_cast<float>(cursorPos.y);


	//マウス座標を `ImGui::Image()` 内の相対座標に変換
	float localMouseX = mousePos.x - viewportPos.x;
	float localMouseY = mousePos.y - (viewportPos.y - viewportSize.y);

	//デバッグ用
	/*ImGui::Text("x = %.3f", localMouseX);
	ImGui::Text("y = %.3f", localMouseY);

	ImGui::Text("x = %.3f", mousePos.x);
	ImGui::Text("y = %.3f", mousePos.y);*/

	//マウス座標を NDC に変換（-1.0 ～ 1.0）
	ndcX = (localMouseX / viewportSize.x) * 2.0f - 1.0f;
	ndcY = 1.0f - (localMouseY / viewportSize.y) * 2.0f;  // Y は逆転

	ImGui::End();

	
}

void MapCreateScene::SaveMap(const std::string& filename)
{

	//セーブファイル
	std::ofstream file(filename);

	//CSVのヘッダーを書き込む
	file << "ObjectName,Position,Size,Rotation,fileName\n";

	//オブジェクトをCSV形式で保存
	
	for (auto obj : m_GameObjectList[1])
	{

		file << obj->GetTag() << ",";
		if (obj->GetTag() == "MeshField")
		{
			MeshField* m = dynamic_cast<MeshField*>(obj);
			m->SaveMeshField("field1.csv");
			file << m->GetFileName() << "\n";
			continue;
		}


		XMFLOAT3 pos;
		XMFLOAT3 size;
		XMFLOAT3 rot;
		if (obj->GetComponent<Transform>() != nullptr)
		{
			Transform* trans = obj->GetComponent<Transform>();
			pos = trans->GetPosition();
			size = trans->GetSize();
			rot = trans->GetRotation();

			file << pos.x << "," << pos.y << "," << pos.z <<","
				  <<size.x << "," << size.y << "," << size.z <<","
				 <<rot.x << "," << rot.y << "," << rot.z << "\n";
		}
		
	}


	file.close();

}

void MapCreateScene::LoadMap(const std::string& filename)
{
}

void MapCreateScene::CopyGameObject()
{
	Transform* editTrans = m_EditObject->GetComponent<Transform>();
	XMFLOAT3 pos = editTrans->GetPosition();
	XMFLOAT3 size = editTrans->GetSize();
	XMFLOAT3 rot = editTrans->GetRotation();


	if (m_EditObject->GetTag() == "Block")
	{
		AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransform(pos,rot,size);

	}
	if (m_EditObject->GetTag() == "Tree")
	{
		AddGameObject<Tree>(1)->GetComponent<Transform>()->SetTransform(pos, rot, size);

	}

}

void MapCreateScene::CopyInstallMeshField(XMFLOAT3 pos)
{

	Transform* editTrans = m_EditObject->GetComponent<Transform>();
	XMFLOAT3 size = editTrans->GetSize();
	XMFLOAT3 rot = editTrans->GetRotation();

	XMFLOAT3 newPos = { pos.x,pos.y + size.y,pos.z };
	if (m_EditObject->GetTag() == "Block")
	{
		AddGameObject<Block>(1)->GetComponent<Transform>()->SetTransform(newPos, rot, size);

	}
	if (m_EditObject->GetTag() == "Tree")
	{
		AddGameObject<Tree>(1)->GetComponent<Transform>()->SetTransform(newPos, rot, size);

	}

}



