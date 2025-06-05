
#include "main.h"
#include "meshField.h"
#include "gameScene.h"
#include "scene.h"
#include "player.h"
#include "transform3DAnimationComponent.h"
#include "texturePool.h"
#include "audio.h"
#include "editorCamera.h"
#include "mapCreateScene.h"
#include <algorithm>


void MeshField::Init()
{
	m_FieldX = 100;
	m_FieldZ = 100;
	m_VertexSpace = 5.0f;

	m_VertexData.resize(m_FieldX,std::vector<VERTEX_3D>(m_FieldZ));
	//頂点バッファ生成
	{
		for (int x = 0; x < m_FieldX; x++)//
		{
			for (int z = 0; z < m_FieldZ; z++)//
			{
				m_VertexData[x][z].Position = XMFLOAT3((x - (m_FieldX/2)) * m_VertexSpace, 0.0f, (z - (m_FieldZ/2)) * -m_VertexSpace);
				m_VertexData[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_VertexData[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_VertexData[x][z].TexCoord = XMFLOAT2(x, z);
			}
		}

		if (m_FieldX > 1 && m_FieldZ > 1)
		{
			for (int x = 1; x < (m_FieldX - 1); x++)//
			{
				for (int z = 1; z < (m_FieldZ - 1); z++)//
				{
					XMFLOAT3 vx, vz, vn;
					vx.x = m_VertexData[x + 1][z].Position.x - m_VertexData[x - 1][z].Position.x;
					vx.y = m_VertexData[x + 1][z].Position.y - m_VertexData[x - 1][z].Position.y;
					vx.z = m_VertexData[x + 1][z].Position.z - m_VertexData[x - 1][z].Position.z;

					vz.x = m_VertexData[x][z - 1].Position.x - m_VertexData[x][z + 1].Position.x;
					vz.y = m_VertexData[x][z - 1].Position.y - m_VertexData[x][z + 1].Position.y;
					vz.z = m_VertexData[x][z - 1].Position.z - m_VertexData[x][z + 1].Position.z;

					//外積
					vn.x = vz.y * vx.z - vz.z * vx.y;
					vn.y = vz.z * vx.x - vz.x * vx.z;
					vn.z = vz.x * vx.y - vz.y * vx.x;

					//正規化(長さ１にする)
					float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
					vn.x /= len;
					vn.y /= len;
					vn.z /= len;

					m_VertexData[x][z].Normal = vn;

				}

			}
		}
		std::vector<VERTEX_3D> vertexFlatData;

		//二次元配列をフラットにする
		for (int x = 0; x < m_FieldX; x++)
		{
			for (int z = 0; z < m_FieldZ; z++)
			{
				vertexFlatData.push_back(m_VertexData[x][z]);
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * vertexFlatData.size();//
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertexFlatData.data();

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ
	{

		int i = 0;
		//インデックスバッファのデータ登録
		for (int x = 0; x < m_FieldX - 1; x++)//
		{
			for (int z = 0; z < m_FieldZ; z++)//
			{
				m_Index.push_back(x * m_FieldX + z);

				m_Index.push_back((x + 1) * (m_FieldX)+z);
			}
			if (x == (m_FieldX - 2))//
				break;
			m_Index.push_back((x + 1) * (m_FieldX)+(m_FieldZ - 1));

			m_Index.push_back((x + 1) * (m_FieldX));

		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * m_Index.size();//
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Index.data();


		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	// **ポリゴンごとのブレンド情報を初期化**
	m_PolygonBlendMap.resize(m_FieldX - 1, std::vector<PolygonBlendInfo>(m_FieldZ - 1));
	
	for (int x = 0; x < m_FieldX - 1; x++)
	{
		for (int z = 0; z < m_FieldZ - 1; z++)
		{
			for (int i = 0; i < 4; i++)
			{
				m_PolygonBlendMap[x][z].blendFactor[i] = 0.5f; // 初期値は0（ブレンドなし）
				m_PolygonBlendMap[x][z].vertices[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// フラットな一次元vectorを作成
	std::vector<PolygonBlendInfo> flatPolygonBlendMap;
	flatPolygonBlendMap.resize((m_FieldX - 1)* (m_FieldZ - 1));

	// 二次元から一次元にデータをコピー
	for (int x = 0; x < m_FieldX - 1; x++)
	{
		for (int z = 0; z < m_FieldZ - 1; z++)
		{
			int index = x * (m_FieldZ - 1) + z; // 一次元配列用のインデックス
			flatPolygonBlendMap[index] = m_PolygonBlendMap[x][z]; // コピー
		}
	}

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // GPUからCPUで更新するならDYNAMIC
	bufferDesc.ByteWidth = sizeof(PolygonBlendInfo) * (m_FieldX - 1) * (m_FieldZ - 1);
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPUから書き込むため
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(PolygonBlendInfo);

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = flatPolygonBlendMap.data(); // データ転送

	//バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bufferDesc, &initData, &m_BlendFactorBuffer);
	
	//シェーダーリソースビュー生成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = (m_FieldX - 1) * (m_FieldZ - 1);

	Renderer::GetDevice()->CreateShaderResourceView(m_BlendFactorBuffer, &srvDesc, &m_BlendMapSRV);

	// **PolygonDataBuffer の定数バッファを作成**
	D3D11_BUFFER_DESC hitbd{};
	hitbd.Usage = D3D11_USAGE_DYNAMIC;
	hitbd.ByteWidth = sizeof(PolygonDataBuffer);
	hitbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hitbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hitbd.MiscFlags = 0;
	hitbd.StructureByteStride = 0; // 定数バッファなので 0

	Renderer::GetDevice()->CreateBuffer(&hitbd, nullptr, &m_PolygonDataBuffer);


	//ブレンドされたテクスチャの生成
	InitBlendMap();
	

	//シェーダー側に送るpolygonデータの初期化
	InitPolygonData();


	//ブレンド用テクスチャー
	m_BaseTextureSRV = TexturePool::GetInstance()->GetTexture("Jimen");
	m_BlendTexture1SRV = TexturePool::GetInstance()->GetTexture("Ice");
	m_BlendTexture2SRV = TexturePool::GetInstance()->GetTexture("Ice");

	m_Texture = TexturePool::GetInstance()->GetTexture("Ice");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");
	Renderer::CreatePixelShader(&m_BlendPS, "shader\\blendMapingPS.cso");
	Renderer::CreateComputeShader(&m_BlendCS, "shader\\blendMapingCS.cso");
	Renderer::CreateComputeShader(&m_FieldBlendCS, "shader\\blendMapingCS.cso");


	Audio* audio = AddComponent<Audio>();
	audio->InitMaster();

	audio->Load("asset\\Audio\\gameBGM.wav");

	audio->SetVolume(0.1f);
	audio->Play(1); 

	Tag = "MeshField";

	D3D11_RASTERIZER_DESC framerasterDesc = {};
	framerasterDesc.FillMode = D3D11_FILL_SOLID; // 通常モード
	framerasterDesc.CullMode = D3D11_CULL_BACK;      // カリングなし（両面描画）
	framerasterDesc.DepthClipEnable = TRUE;

	Renderer::GetDevice()->CreateRasterizerState(&framerasterDesc, &m_NormalState);

	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME; // ワイヤーフレームモード
	rasterDesc.CullMode = D3D11_CULL_BACK;      // カリングなし（両面描画）
	rasterDesc.DepthClipEnable = TRUE;

	Renderer::GetDevice()->CreateRasterizerState(&rasterDesc, &m_WireframeState);
	
	Renderer::CreatePixelShader(&m_FramePixelShader, "shader\\wireFramePS.cso");

}

void MeshField::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void MeshField::Update()
{
	if (m_EditMode == true)
	{

		EditorCamera* camera = Scene::GetInstance()->GetScene<MapCreateScene>()->GetGameObject<EditorCamera>();

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)  // 左クリック
		{
			XMFLOAT3 rayDir = camera->GetMouseRayDirection();
			XMFLOAT3 cameraPos = camera->GetComponent<Transform>()->GetPosition();
			XMFLOAT3 hitPosition;
			RayHitInfo hit;

			//ブレンドマップモード
			if (m_BlendShadingMode == true)
			{
				if (RaycastQuad(cameraPos,rayDir,hit))
				{
					UpdateBlendMapTexture(hit);
				}
			
				return;//シェーディングモードだったら頂点の変更はしない
			}

			if (Raycast(cameraPos, rayDir, hitPosition))
			{

				UpVertex(cameraPos, rayDir, hitPosition);
			}
		}
		else
		{
			m_PolygonData.hitUV = { 0.0f,0.0f };
			m_PolygonData.brushRadius = 0.0f;
			//UpdatePolygonDataBuffer();
		}
	}
}

void MeshField::Draw()
{


	//シェーダー設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_FramePixelShader, NULL, 0);
	Renderer::GetDeviceContext()->RSSetState(m_WireframeState); // ラスタライザーステートを設定

	if (m_EditMode == true)
	{
		
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

		//インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		//テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//Renderer::SetDepthEnable(false);

		//ポリゴン設定
		//Renderer::GetDeviceContext()->Draw(21 * 21, 0);
		Renderer::GetDeviceContext()->DrawIndexed(((m_FieldX + 1) * 2) * (m_FieldZ - 1) - 2, 0, 0);

	}

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->PSSetShader(m_BlendPS,NULL, 0);
	Renderer::GetDeviceContext()->RSSetState(m_NormalState); // ラスタライザーステートを設定

	if (m_ViewState == true)
	{

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

		//インデックスバッファ設定
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		material.TextureEnable = true;
		Renderer::SetMaterial(material);


		// Compute Shader のスレッドグループ数を計算 (512x512 のテクスチャを処理)
		const int THREAD_GROUP_SIZE = 8;
		int dispatchX = BLEND_MAP_RESOLUTION / THREAD_GROUP_SIZE;
		int dispatchY = BLEND_MAP_RESOLUTION / THREAD_GROUP_SIZE;

		Renderer::GetDeviceContext()->CSSetShader(m_BlendCS, NULL, 0);
		//CS側にデータを転送
		Renderer::GetDeviceContext()->CSSetConstantBuffers(0, 1, &m_PolygonDataBuffer);
		Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_BlendMapSRV);
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_BlendTextureUAV, nullptr);

		// Compute Shader の実行
		Renderer::GetDeviceContext()->Dispatch(dispatchX, dispatchY, 1);

		ID3D11UnorderedAccessView* nullUAV = nullptr;
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, &nullUAV, nullptr);


		//// Compute Shader のスレッドグループ数を計算 (512x512 のテクスチャを処理)
		//const int FTHREAD_GROUP_SIZE = 20;
		//int FdispatchX = (BLEND_MAP_RESOLUTION * (m_FieldX - 1)) / FTHREAD_GROUP_SIZE;
		//int FdispatchY = (BLEND_MAP_RESOLUTION * (m_FieldZ - 1)) / FTHREAD_GROUP_SIZE;
		//
		////フィールドのテクスチャを計算
		//Renderer::GetDeviceContext()->CSSetShader(m_FieldBlendCS, NULL, 0);
		//Renderer::GetDeviceContext()->CSSetConstantBuffers(0, 1, &m_PolygonDataBuffer);
		//Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_FieldMapUAV, nullptr);

		//// Compute Shader の実行
		//Renderer::GetDeviceContext()->Dispatch(FdispatchX, FdispatchY, 1);

		//ID3D11UnorderedAccessView* FnullUAV = nullptr;
		//Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &FnullUAV, nullptr);
		//Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, &FnullUAV, nullptr);

		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_BaseTextureSRV);
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_BlendTexture1SRV);

		// **ブレンドデータバッファをピクセルシェーダーにセット**
		Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &m_BlendTextureSRV);
		//Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, &m_PolygonIDMapSRV);

		//Renderer::SetDepthEnable(false);

		//ポリゴン設定
	    Renderer::GetDeviceContext()->DrawIndexed(m_Index.size(), 0, 0);
	}
	

}

void MeshField::DrawImGui()
{
	static  float speed = 0.1f;
	ImGui::DragFloat("speed", &speed, 0.1f, -10.0f, 10.0f);
	m_AddEditVertexSpeed = speed;

	ImGui::Text("range = %d", m_EditSelectRange);
	static int range = 0;
	ImGui::DragInt("range", &range, 1, 0, m_RangeMax);
	m_EditSelectRange = range;

	static float impact = 0;
	ImGui::DragFloat("impact", &impact, 0.05f, 0.0f, 1.0f);
	m_Impact = impact;
	//テクスチャ変更
	ID3D11ShaderResourceView* pTextureSRV = TexturePool::GetInstance()->GetTexture("Ground");
	ImGui::Image(reinterpret_cast<ImTextureID>(pTextureSRV),ImVec2(40.0f, 40.0));
	if (ImGui::Button("Ground"))
	{
		m_Texture = TexturePool::GetInstance()->GetTexture("Ground");
	}

	pTextureSRV = TexturePool::GetInstance()->GetTexture("Ice");
	ImGui::Image(reinterpret_cast<ImTextureID>(pTextureSRV), ImVec2(40.0f, 40.0));
	if (ImGui::Button("Ice"))
	{
		m_Texture = TexturePool::GetInstance()->GetTexture("Ice");
	}

	static bool state = m_BlendShadingMode;
	ImGui::Checkbox("BlendMode", &state);
	m_BlendShadingMode = state;

	static float brushStrength = 0.1f;
	static float brushSize = 0.1f;
	static XMFLOAT3 brushColor = XMFLOAT3(1.0f, 0.0f, 0.0f); //初期設定: 砂 (赤)


	ImGui::SliderFloat("Brush Strength", &brushStrength, 0.01f, 20.0f);
	m_BrushStrength = brushStrength;

	ImGui::SliderFloat("Brush Size", &brushSize, 0.01f, 20.0f);
	m_BrushSize = brushSize;
	ImGui::DragFloat3("Brush Type", (float*)&brushColor, 0.01f, 0.0f, 1.0f);
	m_Color = brushColor;

	if (ImGui::Button("Reset"))
	{
		for (int x = 0; x < m_FieldX; x++)//
		{
			for (int z = 0; z < m_FieldZ; z++)//
			{
				m_VertexData[x][z].Position = XMFLOAT3((x - 10) * m_VertexSpace, 0.0f, (z - 10) * -m_VertexSpace);
				m_VertexData[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_VertexData[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_VertexData[x][z].TexCoord = XMFLOAT2(x, z);
			}
		}

		UpdateVertexBuffer();
	}



	if (ImGui::Button("Save Mesh Field Date"))
	{
		SaveMeshField("field1.csv");
	}
	if (ImGui::Button("Load Mesh Field Data"))
	{
		LoadMeshField("field1.csv");
	}

	if (m_EditMode == true)
	{
		static bool state = m_ViewState;
		ImGui::Checkbox("ViewState", &state);
		m_ViewState = state;
	}

}

void MeshField::UpdateVertexBuffer()
{

	std::vector<VERTEX_3D> vertexFlatData;

	//二次元配列をフラットにする
	for (int x = 0; x < m_FieldX; x++)
	{
		for (int z = 0; z < m_FieldZ; z++)
		{
			vertexFlatData.push_back(m_VertexData[x][z]);
		}
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, vertexFlatData.data(), sizeof(VERTEX_3D) * vertexFlatData.size());
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void MeshField::InitPolygonData()
{


	// Polygon ID を格納するテクスチャを作成
	D3D11_TEXTURE2D_DESC polygonIDDesc = {};
	polygonIDDesc.Width = BLEND_MAP_RESOLUTION;
	polygonIDDesc.Height = BLEND_MAP_RESOLUTION;
	polygonIDDesc.MipLevels = 1;
	polygonIDDesc.ArraySize = 1;
	polygonIDDesc.Format = DXGI_FORMAT_R32_UINT;  // 32bit 整数型
	polygonIDDesc.SampleDesc.Count = 1;
	polygonIDDesc.Usage = D3D11_USAGE_DEFAULT;
	polygonIDDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	polygonIDDesc.CPUAccessFlags = 0;
	polygonIDDesc.MiscFlags = 0;

	ID3D11Texture2D* polygonIDTexture = nullptr;

}

void MeshField::InitBlendMap()
{
	
	//ブレンドした最終テクスチャ
	D3D11_TEXTURE2D_DESC blendDesc{};
	blendDesc.Width = BLEND_MAP_RESOLUTION;
	blendDesc.Height = BLEND_MAP_RESOLUTION;
	blendDesc.MipLevels = 1;
	blendDesc.ArraySize = 1;
	blendDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	blendDesc.SampleDesc.Count = 1;
	blendDesc.Usage = D3D11_USAGE_DEFAULT;
	blendDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	blendDesc.CPUAccessFlags = 0;
	blendDesc.MiscFlags = 0;

	// テクスチャ作成
	HRESULT hr = Renderer::GetDevice()->CreateTexture2D(&blendDesc, nullptr, &m_BlendTexture);
	if (FAILED(hr)) return;

	// シェーダーリソースビュー (SRV)
	D3D11_SHADER_RESOURCE_VIEW_DESC tsrvDesc{};
	tsrvDesc.Format = blendDesc.Format;
	tsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tsrvDesc.Texture2D.MipLevels = 1;
	Renderer::GetDevice()->CreateShaderResourceView(m_BlendTexture, &tsrvDesc, &m_BlendTextureSRV);

	// アンオーダードアクセスビュー (UAV)
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = blendDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	Renderer::GetDevice()->CreateUnorderedAccessView(m_BlendTexture, &uavDesc, &m_BlendTextureUAV);


}


float MeshField::GetHeight(XMFLOAT3 pos)
{
	int x, z;
	//ブロック番号算出
	x = pos.x / m_VertexSpace + (m_FieldX /2);		//Initで設定してる値
	z = pos.z /  - m_VertexSpace + (m_FieldZ /2);

	int xLen = m_VertexData.size();
	if (x  >= xLen - 1)
	{
		return 0.0f;
	}

	int zLen = m_VertexData[x].size();
	
	if (z >= zLen - 1)
	{
		return 0.0f;
	}


	XMFLOAT3 pos0, pos1, pos2, pos3;
	pos0 = m_VertexData[x][z].Position;
	pos1 = m_VertexData[x + 1][z].Position;
	pos2 = m_VertexData[x][z + 1].Position;
	pos3 = m_VertexData[x + 1][z + 1].Position;

	XMFLOAT3 v12, v1p;
	v12.x = pos2.x - pos1.x;
	v12.y = pos2.y - pos1.y;
	v12.z = pos2.z - pos1.z;

	v1p.x = pos.x - pos1.x;
	v1p.y = pos.y - pos1.y;
	v1p.z = pos.z - pos1.z;

	//外積
	float cy = v12.z * v1p.x - v12.x * v1p.z;

	float py;
	XMFLOAT3 n;
	if (cy > 0.0f)
	{
		//左上ポリゴン
		XMFLOAT3 v10;
		v10.x = pos0.x - pos1.x;
		v10.y = pos0.y - pos1.y;
		v10.z = pos0.z - pos1.z;

		//外積
		n.x = v10.y * v12.z - v10.z * v12.y;
		n.y = v10.z * v12.x - v10.x * v12.z;
		n.z = v10.x * v12.y - v10.y * v12.x;
	}
	else
	{
		//右上ポリゴン
		XMFLOAT3 v13;
		v13.x = pos3.x - pos1.x;
		v13.y = pos3.y - pos1.y;
		v13.z = pos3.z - pos1.z;

		//外積
		n.x = v12.y * v13.z - v12.z * v13.y;
		n.y = v12.z * v13.x - v12.x * v13.z;
		n.z = v12.x * v13.y - v12.y * v13.x;
	}

	//高さ取得
	py = -((pos.x - pos1.x) * n.x + (pos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}

bool MeshField::IsMouseOverField(XMFLOAT3 mousePos)
{
	return false;
}

bool MeshField::RayIntersectsTriangle(XMFLOAT3 rayOrigin, XMFLOAT3 rayDir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, float& t)
{
	const float EPSILON = 0.000001f;
	XMFLOAT3 edge1, edge2, h, s, q;
	float a, f, u, v;

	edge1 = XMFLOAT3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
	edge2 = XMFLOAT3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

	XMVECTOR rayDirV = XMLoadFloat3(&rayDir);
	XMVECTOR edge1V = XMLoadFloat3(&edge1);
	XMVECTOR edge2V = XMLoadFloat3(&edge2);
	XMVECTOR hV = XMVector3Cross(rayDirV, edge2V);

	XMStoreFloat3(&h, hV);
	a = edge1.x * h.x + edge1.y * h.y + edge1.z * h.z;

	if (a > -EPSILON && a < EPSILON) return false;

	f = 1.0f / a;
	s = XMFLOAT3(rayOrigin.x - v0.x, rayOrigin.y - v0.y, rayOrigin.z - v0.z);
	u = f * (s.x * h.x + s.y * h.y + s.z * h.z);

	if (u < 0.0f || u > 1.0f) return false;

	XMVECTOR qV = XMVector3Cross(XMLoadFloat3(&s), edge1V);
	XMStoreFloat3(&q, qV);

	v = f * (rayDir.x * q.x + rayDir.y * q.y + rayDir.z * q.z);
	if (v < 0.0f || u + v > 1.0f) return false;

	t = f * (edge2.x * q.x + edge2.y * q.y + edge2.z * q.z);
	return t > EPSILON;
}


bool MeshField::Raycast(XMFLOAT3 rayOrigin, XMFLOAT3 rayDir, XMFLOAT3& hitPosition)
{
	float closestT = FLT_MAX;
	bool hit = false;

	for (int x = 0; x < m_FieldX - 1; x++)
	{
		for (int z = 0; z < m_FieldZ - 1; z++)
		{
			//TriangleStrip の三角形の作り方に基づいて頂点を取得
			XMFLOAT3 v0 = m_VertexData[x][z].Position;
			XMFLOAT3 v1 = m_VertexData[x + 1][z].Position;
			XMFLOAT3 v2 = m_VertexData[x][z + 1].Position;
			XMFLOAT3 v3 = m_VertexData[x + 1][z + 1].Position;

			float t;
			//`TriangleStrip` の場合、三角形の作り方が通常とは異なる
			if ((x + z) % 2 == 0)
			{
				// 偶数ストライプ (左上 -> 右上 -> 左下)
				if (RayIntersectsTriangle(rayOrigin, rayDir, v0, v1, v2, t) && t < closestT)
				{
					closestT = t;
					hitPosition = { rayOrigin.x + rayDir.x * t, rayOrigin.y + rayDir.y * t, rayOrigin.z + rayDir.z * t };
					hit = true;
				}
				// 右上 -> 右下 -> 左下
				if (RayIntersectsTriangle(rayOrigin, rayDir, v1, v3, v2, t) && t < closestT)
				{
					closestT = t;
					hitPosition = { rayOrigin.x + rayDir.x * t, rayOrigin.y + rayDir.y * t, rayOrigin.z + rayDir.z * t };
					hit = true;
				}
			}
			else
			{
				// 奇数ストライプ (左上 -> 右下 -> 左下)
				if (RayIntersectsTriangle(rayOrigin, rayDir, v0, v3, v2, t) && t < closestT)
				{
					closestT = t;
					hitPosition = { rayOrigin.x + rayDir.x * t, rayOrigin.y + rayDir.y * t, rayOrigin.z + rayDir.z * t };
					hit = true;
				}
				// 左上 -> 右上 -> 右下
				if (RayIntersectsTriangle(rayOrigin, rayDir, v0, v1, v3, t) && t < closestT)
				{
					closestT = t;
					hitPosition = { rayOrigin.x + rayDir.x * t, rayOrigin.y + rayDir.y * t, rayOrigin.z + rayDir.z * t };
					hit = true;
				}
			}
		}
	}

	return hit;
}


void MeshField::SaveMeshField(const std::string& filename)
{

	m_FileName = filename;
	//セーブファイル
	std::ofstream file(filename);

	//CSVのヘッダーを書き込む
	file << "x,y,z\n";
	file << m_FieldX << "," << m_FieldZ << "\n";

	//各頂点の座標をCSV形式で保存
	for (int x = 0; x < m_FieldX; x++)
	{
		for (int z = 0; z < m_FieldZ; z++)
		{
			file << m_VertexData[x][z].Position.x << ","
				<< m_VertexData[x][z].Position.y << ","
				<< m_VertexData[x][z].Position.z << ","
				//法線情報
				<< m_VertexData[x][z].Normal.x << ","
				<< m_VertexData[x][z].Normal.y << ","
				<< m_VertexData[x][z].Normal.z << "\n";
		}
	}

	file.close();

}

void MeshField::LoadMeshField(const std::string& filename)
{

	m_FileName = filename;
	std::ifstream file(filename);
	if (!file) return;

	std::string line;
	std::getline(file, line);  //ヘッダー行を読み飛ばす (x,y,z)

	//フィールドのサイズを取得
	std::getline(file, line);
	std::stringstream ss(line);
	std::string fieldxStr, fieldzStr;

	std::getline(ss, fieldxStr, ',');
	std::getline(ss, fieldzStr, ',');

	m_FieldX = std::stof(fieldxStr);
	m_FieldZ = std::stof(fieldzStr);

	m_VertexData.clear();
	m_VertexData.resize(m_FieldX, std::vector<VERTEX_3D>(m_FieldZ));

	std::vector<VERTEX_3D> vertices;
	while (std::getline(file, line))
	{
		VERTEX_3D v;
		std::stringstream ss(line);
		std::string xStr, yStr, zStr;
		std::string xNormal, yNormal, zNormal;

		//座標
		std::getline(ss, xStr, ',');
		std::getline(ss, yStr, ',');
		std::getline(ss, zStr, ',');
		//法線情報
		std::getline(ss, xNormal, ',');
		std::getline(ss, yNormal, ',');
		std::getline(ss, zNormal, ',');


		v.Position.x = std::stof(xStr);
		v.Position.y = std::stof(yStr);
		v.Position.z = std::stof(zStr);

		v.Normal.x = std::stof(xNormal);
		v.Normal.y = std::stof(yNormal);
		v.Normal.z = std::stof(zNormal);

		vertices.push_back(v);
	}
	file.close();

	// ロードしたデータを `m_Vertex` に反映
	int index = 0;
	for (int x = 0; x < m_FieldX; x++)
	{
		for (int z = 0; z < m_FieldZ; z++)
		{
			if (index < vertices.size())
			{
				m_VertexData[x][z] = vertices[index];
				
				m_VertexData[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_VertexData[x][z].TexCoord = XMFLOAT2(x, z);
				index++;
			}
		}
	}

	//バッファを更新
	UpdateVertexBuffer();

}


void MeshField::UpVertex(XMFLOAT3 rayOrigin, XMFLOAT3 rayDir, XMFLOAT3& hitPosition)
{

	// 一番近い頂点を探す
	int closestX = 0, closestZ = 0;
	float minDistance = FLT_MAX;

	for (int x = 0; x < m_FieldX; x++)
	{
		for (int z = 0; z < m_FieldZ; z++)
		{
			float distance = sqrtf(
				pow(hitPosition.x - m_VertexData[x][z].Position.x, 2) +
				pow(hitPosition.z - m_VertexData[x][z].Position.z, 2));

			if (distance < minDistance)
			{
				minDistance = distance;
				closestX = x;
				closestZ = z;
			}
		}
	}

	//法線ベクトル計算
	if (closestX >= 1 && closestX + 1 < m_FieldX && closestZ >= 1 && closestZ + 1 < m_FieldZ)
	{
		XMFLOAT3 vx, vz, vn;
		vx.x = m_VertexData[closestX + 1][closestZ].Position.x - m_VertexData[closestX - 1][closestZ].Position.x;
		vx.y = m_VertexData[closestX + 1][closestZ].Position.y - m_VertexData[closestX - 1][closestZ].Position.y;
		vx.z = m_VertexData[closestX + 1][closestZ].Position.z - m_VertexData[closestX - 1][closestZ].Position.z;

		vz.x = m_VertexData[closestX][closestZ - 1].Position.x - m_VertexData[closestX][closestZ + 1].Position.x;
		vz.y = m_VertexData[closestX][closestZ - 1].Position.y - m_VertexData[closestX][closestZ + 1].Position.y;
		vz.z = m_VertexData[closestX][closestZ - 1].Position.z - m_VertexData[closestX][closestZ + 1].Position.z;

		//外積
		vn.x = vz.y * vx.z - vz.z * vx.y;
		vn.y = vz.z * vx.x - vz.x * vx.z;
		vn.z = vz.x * vx.y - vz.y * vx.x;

		//正規化(長さ１にする)
		float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
		vn.x /= len;
		vn.y /= len;
		vn.z /= len;

		m_VertexData[closestX][closestZ].Normal = vn;
	}

	// 頂点の高さを変更
	m_VertexData[closestX][closestZ].Position.y += m_AddEditVertexSpeed;

	//周りの頂点の変更
	if (m_EditSelectRange >= 1 && m_EditSelectRange <= m_RangeMax)
	{
		float impact = m_Impact;

		for (int x = -m_EditSelectRange; x <= m_EditSelectRange; x++)
		{
			if (closestX + x < 0) continue;
			if (closestX + x >= m_FieldX) continue;

			for (int z = -m_EditSelectRange; z <= m_EditSelectRange; z++)
			{
				if (closestZ + z < 0) continue;
				if (closestZ + z >= m_FieldZ) continue;

				if (x == 0 && z == 0)
				{
					continue;
				}
				XMFLOAT2 pos2 = { m_VertexData[closestX + x][closestZ + z].Position.x,m_VertexData[closestX + x][closestZ + z].Position.z };
				XMFLOAT2 pos1 = { m_VertexData[closestX][closestZ].Position.x,m_VertexData[closestX][closestZ].Position.z };

				XMFLOAT2 vec = { pos2.x - pos1.x,pos2.y - pos1.y };
				float length = sqrt((vec.x * vec.x) + (vec.y * vec.y));
				float add = m_VertexSpace / length;
				m_VertexData[closestX + x][closestZ + z].Position.y += (m_AddEditVertexSpeed * (impact * add));

			}
		}

	}
	//頂点バッファを更新
	UpdateVertexBuffer();

}



void MeshField::UpdateBlendMapTexture(RayHitInfo hit)
{
	

	if (!hit.hit) return;
	
	//polygon番号
	int x = hit.polygonX;
	int z = hit.polygonZ;
	// バリセントリック補間でUV座標を求める

	// **Rayが当たったUV座標を保存**
	m_PolygonData.hitUV = XMFLOAT2(hit.hitUV.x, hit.hitUV.y);
	m_PolygonData.brushRadius = m_BrushSize;

	m_PolygonData.hitPolygonIndex[0] = hit.polygonX;
	m_PolygonData.hitPolygonIndex[1] = hit.polygonZ;

	m_PolygonData.hitPolygonID = hit.polygonX * m_FieldX + hit.polygonZ;

	// **PolygonIDを求めて格納**
	//m_PolygonData.hitPolygonID = m_PolygonIDData[x][z];

	UpdatePolygonDataBuffer();

	//ブレンドデータの更新（累積処理を追加）**
    const float blendIncrement = 0.1f; // 追加するブレンド量（適宜調整）

	for (int i = 0; i < 4; i++)
	{
		m_PolygonBlendMap[x][z].blendFactor[i] =
			std::min(m_PolygonBlendMap[x][z].blendFactor[i] + blendIncrement, 1.0f);
	}


	// **ポリゴンブレンドデータをシェーダーに送るための一次元配列を作成**
	std::vector<PolygonBlendInfo> flatPolygonBlendMap((m_FieldX - 1) * (m_FieldZ - 1));

	for (int i = 0; i < m_FieldX - 1; i++)
	{
		for (int j = 0; j < m_FieldZ - 1; j++)
		{
			int index = i * (m_FieldZ - 1) + j;
			flatPolygonBlendMap[index] = m_PolygonBlendMap[i][j];
		}
	}

	// **ブレンドデータをGPUへ転送**
	if (!m_BlendFactorBuffer) return;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_BlendFactorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) return;

	memcpy(mappedResource.pData, flatPolygonBlendMap.data(), sizeof(PolygonBlendInfo) * flatPolygonBlendMap.size());
	Renderer::GetDeviceContext()->Unmap(m_BlendFactorBuffer, 0);

}

void MeshField::UpdatePolygonDataBuffer()
{

	if (!m_PolygonDataBuffer) return;

	// ポリゴン数をセット
	m_PolygonData.polygonCount = (m_FieldX - 1) * (m_FieldZ - 1);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_PolygonDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) return;

	memcpy(mappedResource.pData, &m_PolygonData, sizeof(PolygonDataBuffer));

	Renderer::GetDeviceContext()->Unmap(m_PolygonDataBuffer, 0);

}



bool MeshField::RayIntersectQuad(const XMFLOAT3& rayOrigin, const XMFLOAT3& rayDir, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3, float& t, XMFLOAT2& hitUV)
{
	// まず四角形の平面方程式を求める
	XMFLOAT3 normal;
	float d;
	if (!ComputeQuadPlane(v0, v1, v2, v3, normal, d)) return false;

	// 平面との交差判定
	float denom = normal.x * rayDir.x + normal.y * rayDir.y + normal.z * rayDir.z;
	if (fabs(denom) < 1e-6f) return false; // Rayと平面が平行ならヒットしない

	t = -(normal.x * rayOrigin.x + normal.y * rayOrigin.y + normal.z * rayOrigin.z + d) / denom;
	if (t < 0) return false; // 交点がRayの逆方向ならヒットしない

	// 交点を求める
	XMFLOAT3 hitPoint = {
		rayOrigin.x + t * rayDir.x,
		rayOrigin.y + t * rayDir.y,
		rayOrigin.z + t * rayDir.z
	};

	// 交点が四角形の内側かチェック
	if (!IsPointInsideQuad(hitPoint, v0, v1, v2, v3)) return false;

	// UV補間を行う
	ComputeQuadUV(hitPoint, v0, v1, v2, v3, hitUV);

	return true;
}

bool MeshField::RaycastQuad(const XMFLOAT3& rayOrigin, const XMFLOAT3& rayDir, RayHitInfo& hitInfo)
{

	hitInfo.hit = false;
	float closestT = FLT_MAX;

	for (int x = 0; x < m_FieldX - 1; x++)
	{
		for (int z = 0; z < m_FieldZ - 1; z++)
		{
			XMFLOAT3 v0 = m_VertexData[x][z].Position;
			XMFLOAT3 v1 = m_VertexData[x + 1][z].Position;
			XMFLOAT3 v2 = m_VertexData[x][z + 1].Position;
			XMFLOAT3 v3 = m_VertexData[x + 1][z + 1].Position;

			float t;
			XMFLOAT2 hitUV;
			if (RayIntersectQuad(rayOrigin, rayDir, v0, v1, v2, v3, t, hitUV) && t < closestT)
			{
				closestT = t;
				hitInfo.hit = true;
				hitInfo.polygonX = x;
				hitInfo.polygonZ = z;
				hitInfo.hitPosition = {
					rayOrigin.x + rayDir.x * t,
					rayOrigin.y + rayDir.y * t,
					rayOrigin.z + rayDir.z * t
				};

				// **修正した UV 座標を設定**
				hitInfo.hitUV = hitUV;

				hitInfo.polygonVertices[0] = v0;
				hitInfo.polygonVertices[1] = v1;
				hitInfo.polygonVertices[2] = v2;
				hitInfo.polygonVertices[3] = v3;
			}
		}
	}

	return hitInfo.hit;
}



void MeshField::ComputeQuadUV(const XMFLOAT3& hitPoint, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3, XMFLOAT2& hitUV)
{
	// 各頂点の UV を定義
	XMFLOAT2 uv0 = { 0.0f, 0.0f };
	XMFLOAT2 uv1 = { 1.0f, 0.0f };
	XMFLOAT2 uv2 = { 0.0f, 1.0f };
	XMFLOAT2 uv3 = { 1.0f, 1.0f };

	// **バリセントリック補間ではなく、四角形全体の線形補間を行う**
	float u = ((hitPoint.x - v0.x) / (v1.x - v0.x)); // X方向の補間
	float v = ((hitPoint.z - v0.z) / (v2.z - v0.z)); // Z方向の補間

	// UVを補間
	hitUV.x = (1.0f - u) * (1.0f - v) * uv0.x + u * (1.0f - v) * uv1.x + (1.0f - u) * v * uv2.x + u * v * uv3.x;
	hitUV.y = (1.0f - u) * (1.0f - v) * uv0.y + u * (1.0f - v) * uv1.y + (1.0f - u) * v * uv2.y + u * v * uv3.y;


}

bool MeshField::ComputeQuadPlane(const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3, XMFLOAT3& normal, float& d)
{
	// v0 -> v1 ベクトル
	XMFLOAT3 edge1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };

	// v0 -> v2 ベクトル
	XMFLOAT3 edge2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

	// 法線を求める (外積)
	normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
	normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
	normal.z = edge1.x * edge2.y - edge1.y * edge2.x;

	// 法線の長さを求める
	float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	// 長さが0なら計算失敗（点が同一直線上にある場合）
	if (length < 1e-6f) {
		return false;
	}

	// 正規化
	normal.x /= length;
	normal.y /= length;
	normal.z /= length;

	// 平面の d を計算
	d = -(normal.x * v0.x + normal.y * v0.y + normal.z * v0.z);

	return true; // 正常に計算できた場合
}

bool MeshField::IsPointInsideQuad(const XMFLOAT3& point, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3)
{
	// 三角形(v0, v1, v2) 内にあるかチェック
	if (IsPointInsideTriangle(point, v0, v1, v2)) return true;

	// 三角形(v1, v2, v3) 内にあるかチェック
	if (IsPointInsideTriangle(point, v1, v2, v3)) return true;

	return false;
}

bool MeshField::IsPointInsideTriangle(const XMFLOAT3& p, const XMFLOAT3& a, const XMFLOAT3& b, const XMFLOAT3& c)
{
	// 三角形の辺ベクトル
	XMFLOAT3 v0 = { c.x - a.x, c.y - a.y, c.z - a.z };
	XMFLOAT3 v1 = { b.x - a.x, b.y - a.y, b.z - a.z };
	XMFLOAT3 v2 = { p.x - a.x, p.y - a.y, p.z - a.z };

	// バリセントリック座標を求める
	float dot00 = v0.x * v0.x + v0.y * v0.y + v0.z * v0.z;
	float dot01 = v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	float dot02 = v0.x * v2.x + v0.y * v2.y + v0.z * v2.z;
	float dot11 = v1.x * v1.x + v1.y * v1.y + v1.z * v1.z;
	float dot12 = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// u, v が 0~1 の範囲にあれば三角形の内側
	return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}


void MeshField::RayCastFieldHitPosition(std::pair<bool, XMFLOAT3>& data)
{
	data.first = false;
	data.second = { 0.0f,0.0f,0.0f };
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)  // 左クリック
	{
		if (m_Click == true) return;
		EditorCamera* camera = Scene::GetInstance()->GetScene<MapCreateScene>()->GetGameObject<EditorCamera>();
		
		XMFLOAT3 rayDir = camera->GetMouseRayDirection();
		XMFLOAT3 cameraPos = camera->GetComponent<Transform>()->GetPosition();
		XMFLOAT3 hitPosition;
		RayHitInfo hit;

		if (Raycast(cameraPos, rayDir, hitPosition))
		{
			data.first = true;
			data.second = hitPosition;
		}
		m_Click = true;
	}
	else
	{
		m_Click = false;
	}
}





