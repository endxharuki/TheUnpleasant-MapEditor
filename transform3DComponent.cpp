#include "main.h"
#include "transform3DComponent.h"


void Transform3DComponent::Init()
{
	m_Model = new ModelRenderer;

	if(m_ModelData != "なし") m_Model->Load(m_ModelData);
	

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_ShaderVS);
	Renderer::CreatePixelShader(&m_PixelShader, m_ShaderPS);
	ComponentNumber = 2;

}

void Transform3DComponent::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	/*Model->UnloadAll();*/
	delete m_Model;

}

void Transform3DComponent::Update()
{
}

void Transform3DComponent::Draw()
{
	if (m_OnDraw == true) {
		if (m_ModelData != "なし") {
			if (m_Model != nullptr) {

				//入力レイアウト設定
				Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

				/*シェーダ設定*/
				Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
				//Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

				//ワールドマトリクス設定
				XMMATRIX world, scale, rot, trans;

				scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
				trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
				/*XMVECTOR v = XMQuaternionRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

				XMFLOAT4 quta = { 0.0f,0.0f,0.0f,1.0f };



				XMVECTOR quatX = XMQuaternionRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), m_Rotation.x);
				XMVECTOR quatY = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), m_Rotation.y);
				XMVECTOR quatZ = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), m_Rotation.z);

				XMVECTOR quatXY = XMQuaternionMultiply(quatX, quatY);
				XMVECTOR quatXYZ = XMQuaternionMultiply(quatXY, quatZ);

				XMMATRIX r =  XMMatrixRotationQuaternion(XMLoadFloat4(&quta));*/
				XMVECTOR v = XMQuaternionRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
				rot = XMMatrixRotationQuaternion(v);

				//rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
				/*rot = XMMatrixRotationQuaternion(quatXYZ);*/



				m_rot = rot;

				XMFLOAT3 p = { 0,0,0 };
				p.x = rot.r[0].m128_f32[0];

				world = scale * rot * trans;

				Renderer::SetWorldMatrix(world);

				m_Model->Draw();
			}
		}
	}

}
