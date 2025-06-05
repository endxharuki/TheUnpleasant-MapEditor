#pragma once
#include "gameObject.h"
#include <fstream>
#include <iostream>
#include <sstream>

#define BLEND_MAP_RESOLUTION (512)

class MeshField : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	ID3D11PixelShader* m_FramePixelShader{};

	//VERTEX_3D m_Vertex[FIELD_Z][FIELD_Z];

	std::vector<std::vector<VERTEX_3D>> m_VertexData;

	std::string m_FileName = "";

	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Size = { 1.0f,1.0f,1.0f };
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };

	ID3D11RasterizerState* m_WireframeState = nullptr;
	ID3D11RasterizerState* m_NormalState = nullptr;

	std::vector<int> m_Index;
	float m_AddEditVertexSpeed = 0.0f;
	int m_EditSelectRange = 0;
	int m_RangeMax = 10;
	float m_Impact = 0.0f;

	bool m_ViewState = true;
	bool m_BlendShadingMode = false;

	int m_FieldX = 100;
	int m_FieldZ = 100;

	float m_VertexSpace = 5.0f;

	bool m_InstallGameObjectMode = false;
	bool m_Click = false;

	float m_BrushSize = 0.1f;
	float m_BrushStrength = 0.1f;
	XMFLOAT4 m_BrushColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_Color = XMFLOAT3(1.0f, 0.0f, 0.0f);

	ID3D11ComputeShader* m_BlendCS = nullptr;
	ID3D11ComputeShader* m_FieldBlendCS = nullptr;

	ID3D11PixelShader* m_BlendPS = nullptr;
	ID3D11ShaderResourceView* m_BaseTextureSRV = nullptr;  //���̃e�N�X�`��
	ID3D11ShaderResourceView* m_BlendTexture1SRV = nullptr; //���̃e�N�X�`��
	ID3D11ShaderResourceView* m_BlendTexture2SRV = nullptr; //��̃e�N�X�`

	//Ray���΂��ċ�����polygon��Data������悤
	struct RayHitInfo
	{
		bool hit;                   // Ray ���q�b�g�������ǂ���
		int polygonX;               // �q�b�g�����|���S���� X �C���f�b�N�X
		int polygonZ;               // �q�b�g�����|���S���� Z �C���f�b�N�X
		XMFLOAT3 hitPosition;       // ���[���h���W�ł̃q�b�g�ʒu
		XMFLOAT3 polygonVertices[4];// �q�b�g�����|���S���̎O�p�`�̒��_
		XMFLOAT2 hitUV;
	};
	// �|���S�����Ƃ̃u�����h�����i�[����\����
	struct PolygonBlendInfo
	{
		XMFLOAT3 vertices[4];   // **�|���S���̊e���_�̃��[���h���W**
		float blendFactor[4]; // 4�̒��_���Ƃ̃u�����h�W��
	};

	struct PolygonDataBuffer
	{
		XMFLOAT2 hitUV;       // Ray����������UV���W
		float brushRadius;    // �u���V�̉e���͈�
		int hitPolygonID;
		int hitPolygonIndex[2];
		int polygonCount;     //polygon��
		int padding;
	};

	ID3D11Buffer* m_PolygonDataBuffer = nullptr;  // �萔�o�b�t�@
	PolygonDataBuffer m_PolygonData;              // �V�F�[�_�[�ɑ���f�[�^

	// �t�B�[���h�̃|���S�����Ƃ̃u�����h�����Ǘ�
	std::vector<std::vector<PolygonBlendInfo>> m_PolygonBlendMap;
	

	ID3D11ShaderResourceView* m_BlendMapSRV = nullptr;
	ID3D11Buffer* m_BlendFactorBuffer = nullptr; // �V�F�[�_�[�p�̃o�b�t�@
	
	// �u�����h����e�N�X�`��
	ID3D11Texture2D* m_BlendTexture = {};
	ID3D11ShaderResourceView* m_BlendTextureSRV = {};
	ID3D11UnorderedAccessView* m_BlendTextureUAV = {};

	//�|���S��ID��ێ�����
	std::vector<UINT> m_PolygonIDData;
	ID3D11Texture2D* m_PolygonIDMapTexture = nullptr;
	ID3D11ShaderResourceView* m_PolygonIDMapSRV = nullptr;//PS�ɑ��M
	ID3D11UnorderedAccessView* m_PolygonIDMapUAV = nullptr;//CS�ɑ��M
	
	//�ꖇ�̃e�N�X�`����
	ID3D11Texture2D* m_FieldMapTexture = nullptr;
	ID3D11ShaderResourceView* m_FieldMapSRV = nullptr;//PS�ɑ��M
	ID3D11UnorderedAccessView* m_FieldMapUAV = nullptr;//CS�ɑ��M



public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void DrawImGui() override;
	void UpdateVertexBuffer();

	void InitPolygonData();
	void InitBlendMap();

	std::string GetFileName()const { return m_FileName; }

	float GetHeight(XMFLOAT3 pos);
	bool IsMouseOverField(XMFLOAT3 mousePos);
	bool RayIntersectsTriangle(XMFLOAT3 rayOrigin, XMFLOAT3 rayDir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, float& t);
	bool Raycast(XMFLOAT3 rayOrigin, XMFLOAT3 rayDir, XMFLOAT3& hitPosition);

	void SaveMeshField(const std::string& filename);
	void LoadMeshField(const std::string& filename);
	void UpVertex(XMFLOAT3 rayOrigin, XMFLOAT3 rayDir, XMFLOAT3& hitPosition);

	void UpdateBlendMapTexture(RayHitInfo hit);
	void UpdatePolygonDataBuffer();

	bool RayIntersectQuad(const XMFLOAT3& rayOrigin, const XMFLOAT3& rayDir,
		const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3,
		float& t, XMFLOAT2& hitUV);
	bool RaycastQuad(const XMFLOAT3& rayOrigin, const XMFLOAT3& rayDir, RayHitInfo& hitInfo);

	void ComputeQuadUV(
		const XMFLOAT3& hitPoint,
		const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3,
		XMFLOAT2& hitUV);

	bool ComputeQuadPlane(
		const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3,
		XMFLOAT3& normal, float& d);

	bool IsPointInsideQuad(
		const XMFLOAT3& point,
		const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, const XMFLOAT3& v3);
	bool IsPointInsideTriangle(
		const XMFLOAT3& p,
		const XMFLOAT3& a, const XMFLOAT3& b, const XMFLOAT3& c);


	void RayCastFieldHitPosition(std::pair<bool, XMFLOAT3>& data);

};