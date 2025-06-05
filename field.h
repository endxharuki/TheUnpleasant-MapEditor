#pragma once
#include "gameObject.h"

class Field : public GameObject
{
private:


	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_3DTexture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

public:

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};

