cbuffer MatrixBuffer : register(b0)
{
    matrix ViewProjectionMatrix; // ビュー射影行列
}


StructuredBuffer<matrix> BoneMatrices : register(t0);

struct InputBoneDate
{
    float3 position : POSITION; // 頂点位置
    float3 normal : NORMAL; // 法線
    float2 texcoord : TEXCOORD; // テクスチャ座標
    int4 boneIndices : BLENDINDICES; // ボーンインデックス
    float4 boneWeights : BLENDWEIGHT; // ボーンウェイト
	
};

struct OutputBoneDate
{
    float4 position : SV_POSITION; // 変換後の頂点位置
    float3 normal : NORMAL; // 変換後の法線
    float2 texcoord : TEXCOORD; // テクスチャ座標
};


OutputBoneDate main(InputBoneDate date)
{
    OutputBoneDate output;

    // スキニング行列の初期化
    matrix skinMatrix = 0;

    // ボーン行列をSRVから取得してスキニング行列を計算
    for (int i = 0; i < 4; ++i)
    {
        skinMatrix += BoneMatrices[date.boneIndices[i]] * date.boneWeights[i];
    }
    
    // 頂点位置と法線にスキニングを適用
    float4 skinnedPosition = mul(float4(date.position, 1.0f), skinMatrix);
    float4 skinnedNormal = mul(float4(date.normal, 0.0f), skinMatrix);

    // 出力
    output.position = mul(skinnedPosition, ViewProjectionMatrix); // ビュー射影行列を適用
    output.normal = normalize(skinnedNormal.xyz);
    output.texcoord = date.texcoord;

    return output;
}