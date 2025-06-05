#pragma once
#include "texture.h"

class Texture2DComponent : public Texture
{

private:

	float m_Alpha = 1.0f;
	XMFLOAT4 m_Diffuse = { 1.0f, 1.0f, 1.0f, m_Alpha};

public:

	Texture2DComponent() = default;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ReInit() override;

	void SetAlpha(float a) { m_Alpha = a;}
	float GetAlpha() { return m_Alpha; }

	void SetDiffuse(XMFLOAT4 rgba) { m_Diffuse = rgba; }

	VERTEX_3D GetVertex(int i) { return m_Vertex[i]; }
	void SetVertex(VERTEX_3D Vertex[4]);

};

