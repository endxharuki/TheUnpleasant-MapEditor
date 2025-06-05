#include "main.h"
#include "transform2DComponent.h"
#include "camera.h"
#include "scene.h"
#include "gameScene.h"
#include "playerCamera.h"
#include "titleScene.h"
#include "titleCamera.h"

void Transform2DComponent::Init()
{
	m_Size = XMFLOAT3(1.0f, 1.0f, 1.0f);

	if (m_3DTexture != nullptr) {
		if (bilboad == false) {
			//頂点バッファ生成
			D3D11_BUFFER_DESC bd{};
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * 4;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd{};
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = m_Vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

			Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_ShaderVS);
			Renderer::CreatePixelShader(&m_PixelShader, m_ShaderPS);

		}
		else if (bilboad == true) {

			//頂点バッファ生成
			D3D11_BUFFER_DESC bd{};
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * 4;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd{};
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = m_Vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


			Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_ShaderVS);
			Renderer::CreatePixelShader(&m_PixelShader, m_ShaderPS);

		}
	}
}

void Transform2DComponent::Uninit()
{

	m_VertexBuffer->Release();
	//m_3DTexture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Transform2DComponent::Update()
{

}

void Transform2DComponent::Draw()
{
	if (m_OnDraw == true)
	{
		if (m_3DTexture != nullptr) {
			if (bilboad == false) {


				//頂点データ書き換え
				D3D11_MAPPED_SUBRESOURCE msr;
				Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

				VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

				vertex[0] = m_Vertex[0];
				vertex[1] = m_Vertex[1];
				vertex[2] = m_Vertex[2];
				vertex[3] = m_Vertex[3];

				Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
				//入力レイアウト設定
				Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

				//シェーダ設定
				Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
				Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

				//ワールドマトリクス設定
				XMMATRIX world, scale, rot, trans;
				scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
				rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
				trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
				world = scale * rot * trans;
				Renderer::SetWorldMatrix(world);

				//頂点バッファ設定
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

				//マテリアル設定
				MATERIAL material;
				ZeroMemory(&material, sizeof(material));
				material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, m_Alpha);
				material.TextureEnable = true;
				Renderer::SetMaterial(material);

				//テクスチャ設定
				Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_3DTexture);

				//プリミティブトポロジ設定
				Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				//ポリゴン描画
				Renderer::GetDeviceContext()->Draw(4, 0);
			}
			if (bilboad == true) {


				//頂点データ書き換え
				D3D11_MAPPED_SUBRESOURCE msr;
				Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

				VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

				vertex[0] = m_Vertex[0];
				vertex[1] = m_Vertex[1];
				vertex[2] = m_Vertex[2];
				vertex[3] = m_Vertex[3];

				Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

				//入力レイアウト設定
				Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

				//シェーダ設定
				Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
				Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

				XMMATRIX view;

				if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == TITLE)
				{
					view = Scene::GetInstance()->GetScene<TitleScene>()->GetGameObject<TitleCamera>()->GetViewMatrix();

				}
				else if (Scene::GetInstance()->GetCurrentScene()->GetSceneName() == GAME)
				{
					view = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<PlayerCamera>()->GetViewMatrix();

				}


				//ビューの逆行れる
				XMMATRIX invView;
				invView = XMMatrixInverse(nullptr, view);//逆行列
				invView.r[3].m128_f32[0] = 0.0f;
				invView.r[3].m128_f32[1] = 0.0f;
				invView.r[3].m128_f32[2] = 0.0f;

				//ワールドマトリクス設定
				XMMATRIX world, scale, rot, trans;
				scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
				//rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
				trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
				world = scale * invView * trans;
				Renderer::SetWorldMatrix(world);

				//頂点バッファ設定
				UINT stride = sizeof(VERTEX_3D);
				UINT offset = 0;
				Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

				//マテリアル設定
				MATERIAL material;
				ZeroMemory(&material, sizeof(material));
				material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, m_Alpha);
				material.TextureEnable = true;
				Renderer::SetMaterial(material);

				//テクスチャ設定
				Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_3DTexture);

				//プリミティブトポロジ設定
				Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				Renderer::SetDepthEnable(false);
				//ポリゴン描画
				Renderer::GetDeviceContext()->Draw(4, 0);

				Renderer::SetDepthEnable(true);
			}
		}
	}
}

void Transform2DComponent::SetVertex(VERTEX_3D Vertex[4])
{

	m_Vertex[0] = Vertex[0];
	m_Vertex[1] = Vertex[1];
	m_Vertex[2] = Vertex[2];
	m_Vertex[3] = Vertex[3];
}
