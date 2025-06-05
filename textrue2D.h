#pragma once
#include "ui.h"
#include "uiComponent.h"
#include "gameObject.h"

class Texture2D : public UI
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;


	int frameCount = 0;
	VERTEX_3D m_Vertex[4] = {};
	bool OnDraw = true;

public:

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetOnDraw(bool On) { OnDraw = On; }

	VERTEX_3D GetVertex(int i) { return m_Vertex[i]; }
	void SetVertex(VERTEX_3D Vertex[4]);

};


