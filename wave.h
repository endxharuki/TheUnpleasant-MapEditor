#pragma once
#include "gameObject.h"

#define WAVE_X (100)
#define WAVE_Z (100)

class Wave : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	VERTEX_3D m_Vertex[WAVE_X][WAVE_Z];
	float time[WAVE_X][WAVE_Z] = {};

	float m_Current[WAVE_X][WAVE_Z] = {};
	float m_Prev[WAVE_X][WAVE_Z] = {};
	float m_Next[WAVE_X][WAVE_Z] = {};


	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };

	float amp[WAVE_X][WAVE_Z];
	float Time = 0.0f;
	float Amp = 0.1f;

	int step = 0;

	int cameraX = 0;
	int cameraZ = 0;

	bool wave = false;

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	float GetHeight(XMFLOAT3 pos);
	void SetCameraPos(int x,int z)
	{
		cameraX = x;
		cameraZ = z;
		wave = false;
		step = 0;
		Time = 0.0f;
		Amp = 0.01f;
		wave = false;
	}

	// ÉKÉEÉXï™ïzÇèâä˙îgåπÇ∆ÇµÇƒê›íËÇ∑ÇÈä÷êî
	float gaussian(int x, int y, float center_x, float center_y, float sigma) {
		return exp(-((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y)) / (2 * sigma * sigma));
	}

};