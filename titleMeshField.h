#pragma once
#include "gameObject.h"

#define MESH_X (100)
#define MESH_Z (100)

class TitleMeshField : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	VERTEX_3D m_Vertex[MESH_X][MESH_Z];

	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };

	float m_FieldHeight[MESH_X][MESH_Z] = {};

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void DrawImGui() override;


	float GetHeight(XMFLOAT3 pos);

};