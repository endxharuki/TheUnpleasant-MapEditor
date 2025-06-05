#include "main.h"
#include "block.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "scene.h"
#include "gameScene.h"


void Block::Init()
{
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
	Renderer::CreatePixelShader(&m_FramePixelShader, "shader\\wireFramePS.cso");
	AddComponent<Transform3DComponent>()->AddModelData("asset\\Model\\boxTest.obj");
	AddComponent<BoxColiderComponent>();

	GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	GetComponent<Transform3DComponent>()->SetSize(XMFLOAT3(3.0f, 3.0f, 3.0f));

	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	GetComponent<BoxColiderComponent>()->SetSize(XMFLOAT3(size.x / 2, size.y / 2, size.z / 2));
	Tag = "Block";

	//このオブジェクトは静的
	m_GameObjectKind = Static;

	//描画しない
	if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == TITLE)
	{
		GetComponent<Transform3DComponent>()->SetDraw(false);
	}

	/*frame = new Transform3DComponent;
	frame = GetComponent<Transform3DComponent>();*/

}

void Block::Uninit()
{
	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}
}

void Block::Update()
{
	float add = 0.01f;

	//GetComponent<BoxColiderComponent>()->GetCollisionSAT();

	XMFLOAT3 ColiederOldPos = GetComponent<BoxColiderComponent>()->GetPosition();

	XMFLOAT3 TransformPos = GetComponent<Transform3DComponent>()->GetPosition();
	XMFLOAT3 ColiederPos = GetComponent<BoxColiderComponent>()->GetPosition();
	XMFLOAT3 Rotation = GetComponent<Transform>()->GetRotation();
	
	XMFLOAT3 size = GetComponent<Transform3DComponent>()->GetSize();
	GetComponent<BoxColiderComponent>()->SetSize(XMFLOAT3(size.x / 2, size.y / 2, size.z / 2));

	GetComponent<BoxColiderComponent>()->SetPosition(ColiederPos);

	GetComponent<Transform>()->SetRotation(XMFLOAT3(Rotation.x, Rotation.y, Rotation.z));

	for (auto com : ComponentList)
	{
		com->Update();

	}
}

void Block::Draw()
{

	if (m_EditMode == true)
	{
		Renderer::GetDeviceContext()->PSSetShader(m_FramePixelShader, nullptr, 0);

		ID3D11RasterizerState* wireframeState = nullptr;

		D3D11_RASTERIZER_DESC rasterDesc = {};
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME; // ワイヤーフレームモード
		rasterDesc.CullMode = D3D11_CULL_NONE;      // カリングなし（両面描画）
		rasterDesc.DepthClipEnable = TRUE;

		Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &wireframeState);
		Renderer::GetDeviceContext()->RSSetState(wireframeState); // ラスタライザーステートを設定

		for (auto com : ComponentList)
		{
			com->Draw();
		}
	}
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);

	ID3D11RasterizerState* frameState = nullptr;

	D3D11_RASTERIZER_DESC framerasterDesc = {};
	framerasterDesc.FillMode = D3D11_FILL_SOLID; // 通常モード
	framerasterDesc.CullMode = D3D11_CULL_BACK;      // カリングなし（両面描画）
	framerasterDesc.DepthClipEnable = TRUE;

	Renderer::GetDevice()->CreateRasterizerState(&framerasterDesc, &frameState);
	Renderer::GetDeviceContext()->RSSetState(frameState); // ラスタライザーステートを設定
	for (auto com : ComponentList)
	{
		com->Draw();
	}



}

