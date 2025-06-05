#include "common.hlsl"

#define BLEND_MAP_RESOLUTION (512)
#define FIELD_X (19.0)
#define FIELD_Z (19.0)


RWTexture2D<float4> FieldBlendMap : register(u0);//一枚のブレンドにしたバージョン
RWTexture2D<uint> PolygonIDMap : register(u1); // 各ピクセルが属するポリゴンID


// C++ 側から送られるバッファ
cbuffer PolygonDataBuffer : register(b0)
{
    float2 hitUV; // Rayが当たったUV座標
    float brushRadius; // ブラシの影響範囲
    int hitPolygonID; // 16バイトアライメントのためのパディング
    int2 hitPolygonIndex; //polygonのxとzの番号
    int polygonCount; // ポリゴン数
    float padding; // 12バイト (16バイトアライメント確保)
}

// スレッドごとの処理
[numthreads(20, 20, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    
    //一枚のテクスチャ版
    float2 fieldUV = DTid.xy / float2(BLEND_MAP_RESOLUTION * FIELD_X, BLEND_MAP_RESOLUTION * FIELD_Z);
    
    float x = hitPolygonIndex.x;
    float y = hitPolygonIndex.y;
    
    float uvX = (x / FIELD_X);
    float uvY = (y / FIELD_Z);
    
    float addX = hitUV.x / FIELD_X;
    float addY = hitUV.y / FIELD_Z;
    
    float2 FieldUV = (addX + uvX,addY + uvY);
    
    //// 現在のブレンド値を取得
    float4 fieldCurrentFactor = FieldBlendMap[DTid.xy];

    // ブレンドの増加量 (Ray の影響範囲内なら増加)
    float fieldDistance = length(fieldUV - hitUV);
    float fieldIncrease = (fieldDistance < brushRadius) ? 0.02 : 0.0; // 徐々に増やす

    // **過去の値を加算して累積**
    float4 FieldNewFactor = saturate(fieldCurrentFactor + float4(fieldIncrease, fieldIncrease, fieldIncrease, 0));

    // 更新
    FieldBlendMap[DTid.xy] = FieldNewFactor;
    
    
}