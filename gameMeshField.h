#pragma once
#include "gameObject.h"

#define FIELD_X (46)
#define FIELD_Z (46)

class MeshField : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	VERTEX_3D m_Vertex[FIELD_X][FIELD_Z];

	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void DrawImGui() override;


	float GetHeight(XMFLOAT3 pos);

};