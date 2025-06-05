#pragma once
#include "gameObjectComponent.h"


// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL_OBJ
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

struct MODEL
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};


#include "component.h"
#include <string>
#include <unordered_map>


class ModelRenderer : public GameObjectComponent
{
private:

	static std::unordered_map<std::string, MODEL*> m_ModelPool;


	static void LoadModel(const char *FileName, MODEL *Model);
	static void LoadObj( const char *FileName, MODEL_OBJ *ModelObj );
	static void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

	MODEL* m_Model{};

	std::vector<DirectX::XMMATRIX> m_BoneMatrices;//ボーン行列
	ID3D11Buffer* m_BoneBuffer = nullptr;
	ID3D11ShaderResourceView* m_BoneSRV = nullptr;


	struct BoneVertex
	{
		DirectX::XMFLOAT3 Position;  // 頂点位置
		DirectX::XMFLOAT3 Normal;    // 法線
		DirectX::XMFLOAT2 TexCoord;  // テクスチャ座標
		int BoneIndices[4];          // ボーンインデックス（最大4つ）
		float BoneWeights[4];        // ボーンウェイト（最大4つ）
	};

public:

	static void Preload( const char *FileName );
	static void UnloadAll();


	using GameObjectComponent::GameObjectComponent;

	void Load( const char *FileName );

	void Draw() override;

	void CreateBoneBuffer(ID3D11Device* device, int boneCount);

};