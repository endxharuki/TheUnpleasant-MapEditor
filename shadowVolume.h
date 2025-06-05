#pragma once
#include "gameObject.h"
#include "component.h"

class ShadowVolume : public GameObject {
private:
	Component* m_Component{};

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	ID3D11ShaderResourceView* m_EnvTexture{};			//環境マッピング

	XMFLOAT3 m_Position = {0.0f,0.0f,0.0f};
	XMFLOAT3 m_Size = {0.0f,0.0f,0.0f};
	XMFLOAT3 m_Rotation={ 0.0f,0.0f,0.0f };

	XMFLOAT3 m_TargetPosition = { 0.0f,0.0f,0.0f };

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

};