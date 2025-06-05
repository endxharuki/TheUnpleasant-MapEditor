#pragma once
#include "gameObject.h"
#include "component.h"

class ParticleSpowner: public GameObject
{
private:

	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_3DTexture{};

	int frameCount = 0;

	int m_Count{};

	struct PARTICLE
	{

		bool Enable;
		int  Life;
		XMFLOAT3 pos;
		XMFLOAT3 vel;

	};

	static const int PARTICLE_MAX = 1000;
	PARTICLE m_Particle[PARTICLE_MAX]{};

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetPosition(XMFLOAT3 pos) { m_Position = pos; }
	XMFLOAT3 GetPosition() const { return m_Position; }
};


