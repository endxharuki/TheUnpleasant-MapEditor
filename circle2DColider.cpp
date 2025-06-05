#include "main.h"
#include "circle2DColider.h"
#include "texturePool.h"
#include "gameScene.h"
#include "scene.h"
#include "transform2DComponent.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "textrue2D.h"
#include "texture2DComponent.h"

void Circle2DColider::Init()
{

	m_Texture = Scene::GetInstance()->GetScene<GameScene>()->AddUIObject<Texture2D>();
	m_Texture->SetPosTexNumber(m_CenterPosition.x - m_Radius, m_CenterPosition.y - m_Radius, m_Radius, m_Radius, TexturePool::GetInstance()->GetTexture("MagicFrame"), 0);
	m_Texture->SetOnDraw(true);
}

void Circle2DColider::Uninit()
{

}

void Circle2DColider::Update()
{


	if (m_Update == true)
	{

		//スクリーンサイズ
		IDXGISwapChain* swapChain = Renderer::GetSwapChain();
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		swapChain->GetDesc(&swapChainDesc);

		int screenWideth = swapChainDesc.BufferDesc.Width;
		int screenHeight = swapChainDesc.BufferDesc.Height;

		m_CenterPosition = { screenWideth / 2.0f,screenHeight / 2.0f };
		m_Texture->SetVertexPosition(m_CenterPosition.x - m_Radius, m_CenterPosition.y - m_Radius, m_Radius * 2, m_Radius * 2);
		m_TargetPosition = { 0.0f,0.0f };

		InObject = false;
		int UISize = 1;
		int count = 0;;
		//オブジェクトのテクスチャのサイズ
		float vertexSize = 40.0f;
		for (auto obj : Scene::GetInstance()->GetScene<GameScene>()->GetGameObjectList(1))
		{
			if (obj->GetTag() == "Enemy")
			{
				float posX, posY;
				

				XMFLOAT3 pos = obj->GetComponent<Transform>()->GetPosition();
				if (ObjectInCircle(pos, screenWideth, screenHeight,posX,posY))
				{
					pos.y += 1.0f;
					m_TargetWorldPosition = pos;
					m_TargetPosition.x = posX;
					m_TargetPosition.y = posY;

					if (UISize <= m_OldUICount)
					{
						m_MagicUIList[UISize - 1]->SetVertexPosition(posX - vertexSize, posY - vertexSize, vertexSize * 2, vertexSize * 2);
						count++;
					}
					else if (UISize > m_OldUICount)
					{
						Texture2D* tex = Scene::GetInstance()->GetScene<GameScene>()->AddUIObject<Texture2D>();
						tex->Init();
						tex->SetPosTexNumber(posX - vertexSize, posY - vertexSize, vertexSize * 2, vertexSize * 2, TexturePool::GetInstance()->GetTexture("MagicFrame"), 0);
						tex->GetComponent<Texture2DComponent>(0)->SetDiffuse(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
						m_MagicUIList.push_back(tex);
						count++;
					}
					UISize++;
					InObject = true;
				}

			}
		}

		if (count == 0)
		{
			for (auto tex : m_MagicUIList)
			{
				tex->SetDelete(true);
			}
			m_MagicUIList.clear();
			InObject = false;
			m_TargetWorldPosition = { 0.0f,0.0f,0.0f };
		}
		

		m_OldUICount = m_MagicUIList.size();

	}
	else
	{
		for (auto tex : m_MagicUIList)
		{
			tex->SetDelete(true);
		}
		m_MagicUIList.clear();
		InObject = false;
		m_TargetWorldPosition = { 0.0f,0.0f,0.0f };

	}
}

void Circle2DColider::Draw()
{
	if (m_Draw == true)
	{
		m_Texture->SetOnDraw(true);
		DrawImGui();
	}
	else
	{
		m_Texture->SetOnDraw(false);
	}
}

XMFLOAT4 ndc;

bool Circle2DColider::ObjectInCircle(XMFLOAT3 worldPos, int screenWidth, int screenHeight, float& screenPosX, float& screenPosY)
{
	// 1. ワールド座標 -> クリップ座標変換
	worldPos.y += 3.0f;
	XMVECTOR worldPosition = XMLoadFloat3(&worldPos);
	XMVECTOR viewPosition = XMVector3TransformCoord(worldPosition, m_ViewMatrix);
	XMVECTOR clipPosition = XMVector3TransformCoord(viewPosition, m_ProjectionMatrix);


	XMFLOAT4 clipPos;
	XMStoreFloat4(&clipPos, viewPosition);

	ndc = clipPos;

	if (clipPos.z < 0.0f)
	{
		return false;
	}

	// 2. クリップ座標 -> 正規化デバイス座標 (NDC)
	XMFLOAT3 ndcPos;
	XMStoreFloat3(&ndcPos, clipPosition);

	

	//if (ndcPos.z < 0.0f) // カメラ後方にある場合は無視
	//	return false;

	// 3. 正規化デバイス座標 -> スクリーン座標
	screenPosX = (ndcPos.x + 1.0f) * 0.5f * screenWidth;
	screenPosY = (1.0f - ndcPos.y) * 0.5f * screenHeight;

	// 4. スクリーン座標と円の中心の距離を比較
	float distance = sqrtf(powf(screenPosX - m_CenterPosition.x, 2) + powf(screenPosY - m_CenterPosition.y, 2));
	return (distance <= m_Radius);

}

void Circle2DColider::DrawImGui()
{
	if (onImGui == true)
	{

		ImGui::Begin("InCircleObject");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("Radiusu = %.0f", m_Radius);

		ImGui::Text("ndcPos x  = %.0f y = %.0f z = %.0f",ndc.x,ndc.y,ndc.z);

		if (InObject == true)
		{
			ImGui::Text("InObject");
		}
		

		ImGui::End();

	}


}
