#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "gameObjectComponent.h"


//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;
	int				BoneNum;
	std::string		BoneName[4];//本来はボーンインデックスで管理するべき
	float			BoneWeight[4];
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;
};

//ボーンデータ
struct BoneVertex
{
	DirectX::XMFLOAT3 Position;  // 頂点位置
	DirectX::XMFLOAT3 Normal;    // 法線
	DirectX::XMFLOAT2 TexCoord;  // テクスチャ座標
	int BoneIndices[4];          // ボーンインデックス（最大4つ）
	float BoneWeights[4];        // ボーンウェイト（最大4つ）
	DirectX::XMFLOAT4 Diffuse;   // 色
};

class AnimationModel : public GameObjectComponent
{
private:
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;//変形後頂点データ
	std::vector<BoneVertex>* m_DeformBoneVertex;//変形後頂点データ

	std::unordered_map<std::string, BONE> m_Bone;//ボーンデータ（名前で参照）

	ID3D11Buffer* m_BoneBuffer = nullptr;
	ID3D11ShaderResourceView* m_BoneSRV = nullptr;

	ID3D11VertexShader* m_VertexShader{};


	int m_BoneCount = 0;
	float m_Alpha = 0.0f;

public:
	using GameObjectComponent::GameObjectComponent;

	void Load(const char* FileName);
	void Uninit() override;
	void Draw() override;

	void LoadAnimation(const char* FileName, const char* Name);
	void CreateBone(aiNode* node);
	void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRatio);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
	void CreateBoneBuffer(int boneCount);
	void UpdateBoneBuffer(std::vector<aiMatrix4x4> boneMatrices);

	std::unordered_map<std::string, BONE> GetBone() { return m_Bone; }

	void SetAlpha(float a) { m_Alpha = a; }
};