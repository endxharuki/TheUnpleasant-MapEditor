#pragma once
#include "uiComponent.h"
#include "renderer.h"
#include "modelRenderer.h"

class Texture : public UIComponent
{
protected:

	VERTEX_3D m_Vertex[4] = {};

	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ModelRenderer* Model = nullptr;

	float m_VposX = 0.0f, m_VposY = 0.0f;
	float m_VsizeX = 0.0f, m_VsizeY = 0.0f;
	float m_AnimeX = 0.0f, m_AnimeY = 0.0f;
	float m_AsizeX = 0.0f, m_AsizeY = 0.0f;

	const wchar_t* TextureData = L"‚È‚µ";

	int m_FrameCount = 0;


public:

	Texture() { m_Tag = "Texture"; }
	Texture(UI* ui, const wchar_t* date) : UIComponent(ui), TextureData(date) {}


	virtual void Init() override {};
	virtual void Uninit() override {};
	virtual void Update() override {};
	virtual void Draw() override {};
	virtual void ReInit() override {};

	XMFLOAT3 GetPosition() { return m_Position; }
	XMFLOAT3 GetRotation() { return m_Rotation; }
	XMFLOAT3 GetSize() { return m_Size; }
	VERTEX_3D GetVertex(int i) { return m_Vertex[i]; }

	void SetPosition(const XMFLOAT3 pos) { m_Position = pos; }
	void SetSize(const XMFLOAT3 size) { m_Size = size; }
	void SetRotation(const XMFLOAT3 rot) { m_Rotation = rot; }
	void SetTexture(const wchar_t* texture) { TextureData = texture; }
	void SetAnimation(float x, float y, float sizeX, float sizeY)
	{
		m_AnimeX = x;
		m_AnimeY = y;
		m_AsizeX = sizeX;
		m_AsizeY = sizeY;

	}

	void SetPosTexNumber(float posx, float posy, float sizex, float sizey, ID3D11ShaderResourceView* texture,int number)
	{
		m_VposX = posx;
		m_VposY = posy;
		m_VsizeX = sizex;
		m_VsizeY = sizey;

		m_Texture = texture;

		ComponentNumber = number;
	}

	void SetTexturePosSize(float posx, float posy, float sizex, float sizey)
	{
		m_VposX = posx;
		m_VposY = posy;
		m_VsizeX = sizex;
		m_VsizeY = sizey;
	}

};
