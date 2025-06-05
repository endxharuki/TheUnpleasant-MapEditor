#include "main.h"
#include "texture2DComponent.h"
#include "camera.h"
#include "scene.h"
#include "gameScene.h"
#include "playerCamera.h"

void Texture2DComponent::Init()
{

	m_Size = XMFLOAT3(1.0f, 1.0f, 1.0f);


	m_Vertex[0].Position = XMFLOAT3(m_VposX, m_VposY, 0.0f);
	m_Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	m_Vertex[1].Position = XMFLOAT3(m_VposX + m_VsizeX, m_VposY, 0.0f);
	m_Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	m_Vertex[2].Position = XMFLOAT3(m_VposX, m_VposY + m_VsizeY, 0.0f);
	m_Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	m_Vertex[3].Position = XMFLOAT3(m_VposX + m_VsizeX, m_VposY + m_VsizeY, 0.0f);
	m_Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

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

	///*テクスチャ読み込み*/
	//TexMetadata metadata;
	//ScratchImage image;
	//LoadFromWICFile(TextureData, WIC_FLAGS_NONE, &metadata, image);
	//CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	//assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
	
	m_AnimeX = 0.0f;
	m_AnimeY = 0.0f;
	m_AsizeX = 1.0f;
	m_AsizeY = 1.0f;

}

void Texture2DComponent::Uninit()
{

	m_VertexBuffer->Release();
	//テクスチャーは一括管理してるからRelease要らんよ
	//m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Texture2DComponent::Update()
{
	m_Diffuse.w = m_Alpha;
}

void Texture2DComponent::Draw()
{

	//頂点データ書き換え

	m_Vertex[0].Position = XMFLOAT3(m_VposX, m_VposY, 0.0f);
	m_Vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[0].TexCoord = XMFLOAT2(m_AnimeX, m_AnimeY);

	m_Vertex[1].Position = XMFLOAT3(m_VposX + m_VsizeX, m_VposY, 0.0f);
	m_Vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[1].TexCoord = XMFLOAT2(m_AnimeX + m_AsizeX, m_AnimeY);

	m_Vertex[2].Position = XMFLOAT3(m_VposX, m_VposY + m_VsizeY, 0.0f);
	m_Vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[2].TexCoord = XMFLOAT2(m_AnimeX, m_AnimeY + m_AsizeY);

	m_Vertex[3].Position = XMFLOAT3(m_VposX + m_VsizeX, m_VposY + m_VsizeY, 0.0f);
	m_Vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertex[3].TexCoord = XMFLOAT2(m_AnimeX + m_AsizeX, m_AnimeY + m_AsizeY);

	
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	memcpy(vertex, m_Vertex, sizeof(VERTEX_3D) * 4);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Diffuse;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::SetATCEnable(true);

	Renderer::GetDeviceContext()->Draw(4, 0);

	Renderer::SetATCEnable(false);

}

void Texture2DComponent::ReInit()
{

	//頂点データ書き換え

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0] = m_Vertex[0];
	vertex[1] = m_Vertex[1];
	vertex[2] = m_Vertex[2];
	vertex[3] = m_Vertex[3];

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Diffuse;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


}

void Texture2DComponent::SetVertex(VERTEX_3D Vertex[4])
{

	m_Vertex[0] = Vertex[0];
	m_Vertex[1] = Vertex[1];
	m_Vertex[2] = Vertex[2];
	m_Vertex[3] = Vertex[3];

}
