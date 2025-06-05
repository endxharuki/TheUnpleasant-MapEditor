#include "main.h"
#include "ParticleSystem.h"
#include "renderer.h"

void ParticleSystem::Init()
{
	

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}

void ParticleSystem::Uninit()
{


}

void ParticleSystem::Update()
{
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_Uav, nullptr);

	int listSize = particleList.size();

	//パーティクルの更新
	Renderer::GetDeviceContext()->Dispatch(listSize, 1, 1);

	//UAVを解除
	ID3D11UnorderedAccessView* nullUAV = nullptr;
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);


}

void ParticleSystem::Draw()
{
	
	ID3D11RenderTargetView* renderTargetView = Renderer::GetRenderTagetView();

	Renderer::GetDeviceContext()->OMSetRenderTargets(1, &renderTargetView, nullptr);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);

	int listSize = particleList.size();

	Renderer::GetDeviceContext()->Draw(listSize, 0);

}

void ParticleSystem::SetParticle(int count)
{

	for (int i = 0; i < count; i++)
	{
		ParticleData p =
		{
			{0.0f,0.0f,0.0f},
			{1.0f,1.0f,1.0f},
			{0.0f,0.1f,0.0f},
			(rand() % 10) / 10.0f + 5.0f
		};
		particleList.push_back(&p);
	}

}

void ParticleSystem::CreateParticleResorces()
{



}
