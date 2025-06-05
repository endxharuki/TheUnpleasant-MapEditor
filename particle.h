#pragma once
#include "gameObject.h"
#include "component.h"

class Particle: public GameObject
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



public:

	bool Enable = false;;
	int  Life;

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetPosition(XMFLOAT3 pos) { m_Position = pos; }
	XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 vel = { 0.0f,0.0f,0.0f };

};


