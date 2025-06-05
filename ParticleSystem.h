#pragma once
#include "gameObject.h"
#include "GameObjectComponent.h"

class ParticleSystem : public GameObjectComponent
{
private:

	struct ParticleData
	{
		XMFLOAT3 position;
		XMFLOAT3 size;
		XMFLOAT3 velocity;
		float lifeTime = 0.0f;
	};

	ID3D11ComputeShader* m_ComputeShader{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11InputLayout* m_VertexLayout{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11Buffer* m_StructBuffer{};
	ID3D11UnorderedAccessView* m_Uav = nullptr;
	ID3D11ShaderResourceView* m_Srv = nullptr;

	std::list<ParticleData*> particleList;

	float m_ParticleCount = 0.0f;

public:

	void SetLifeTime(float count)
	{
		m_ParticleCount = count;
	}
	float GetLifeTime() const
	{
		return m_ParticleCount;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetParticle(int count);

	void CreateParticleResorces();

};

