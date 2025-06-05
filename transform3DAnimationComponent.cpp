#include "main.h"
#include "transform3DAnimationComponent.h"


void Transform3DAnimationComponent::Init()
{
	
	if (m_GameObject != nullptr) {


		m_Model = new AnimationModel(m_GameObject);
		/*m_Model->Load(m_ModelData);*/

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_ShaderVS);
		Renderer::CreatePixelShader(&m_PixelShader, m_ShaderPS);

		ComponentNumber = 2;

		m_AnimationFrame = 0.0f;
	}
	
}

void Transform3DAnimationComponent::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	/*Model->UnloadAll();*/
	delete m_Model;

}

void Transform3DAnimationComponent::Update()
{
	
	m_Model->Update(m_AnimationName.c_str(), m_AnimationFrame, m_AnimationName.c_str(), m_AnimationFrame, m_AnimationBolendRatio);

	if (m_DelayFrame == m_Delay)
	{
		m_AnimationFrame++;
		m_DelayFrame = 0;
	}
	
	m_DelayFrame++;

	//アニメーションのステートが変わったらAnimationBolendRatioを0にして
	if (m_ChangeState == true)
	{
		m_AnimationBolendRatio += 0.1f;
		if (m_AnimationBolendRatio > 1.0f)
		{
			m_AnimationBolendRatio = 1.0f;
			m_ChangeState = false;
		}
	}

}

void Transform3DAnimationComponent::Draw()
{
	if (m_ModelData != "なし") {
		if (m_Model != nullptr) {

			//入力レイアウト設定
			Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

			/*シェーダ設定*/
			Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
			Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

			//ワールドマトリクス設定
			XMMATRIX world, scale, rot, trans;
			scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
			rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
			trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
			world = scale * rot * trans;

			Renderer::SetWorldMatrix(world);

			m_Model->Draw();
		}
	}
	

}
