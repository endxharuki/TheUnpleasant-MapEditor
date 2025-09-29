#include "common.hlsl"

#define BLEND_MAP_RESOLUTION (512)
#define FIELD_X (19.0)
#define FIELD_Z (19.0)

// ブレンド情報の更新を行うCompute Shader
RWTexture2D<float4> BlendMap : register(u0); // ブレンド情報を格納するテクスチャ
RWTexture2D<float4> FieldBlendMap : register(u1);//一枚のブレンドにしたバージョン
RWTexture2D<uint> PolygonIDMap : register(u2); // 各ピクセルが属するポリゴンID


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
[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    
    //ブレンドマッピングが一応できてる
    float2 uv = DTid.xy / float2(BLEND_MAP_RESOLUTION, BLEND_MAP_RESOLUTION);

    //// 現在のブレンド値を取得
    float4 currentFactor = BlendMap[DTid.xy];

    // ブレンドの増加量 (Ray の影響範囲内なら増加)
    float distance = length(uv - hitUV);
    float increase = (distance < brushRadius) ? 0.02 : 0.0; // 徐々に増やす

    // **過去の値を加算して累積**
    float4 newFactor = saturate(currentFactor + float4(increase, increase, increase, 0));

    // 更新
    BlendMap[DTid.xy] = newFactor;
    
    
}

////失敗
    //uint2 pixelCoord = DTid.xy;
    //float2 uv = pixelCoord / float2(BLEND_MAP_RESOLUTION, BLEND_MAP_RESOLUTION);

    //// 現在のポリゴン ID を取得
    //uint polygonID = PolygonIDMap[pixelCoord];

    //// ヒットしたポリゴンと一致しない場合は何もしない
    //if (polygonID != hitPolygonID)
    //{
    //    //BlendMap[pixelCoord] = float4(0, 0, 1, 1);
    //    return;
    //}

    //// ブレンドの増加量 (Ray の影響範囲内なら増加)
    //float distance = length(uv - hitUV);
    //float increase = (distance < brushRadius) ? 0.02 : 0.0; // 徐々に増やす
   
    //// **現在のブレンド値を取得**
    //float4 currentBlend = BlendMap[pixelCoord];

    ////// **ブレンド強度を計算（中心ほど強くブレンド）**
    ////float blendStrength = 1.0 - saturate(distance / brushRadius);
    ////float blendAmount = 0.05 * blendStrength; // 少しずつブレンド
    
    //// **過去の値を加算して累積**
    //float4 newFactor = saturate(currentBlend + float4(increase, increase, increase, 0));

    ////// **新しいブレンド値を加算**
    ////float4 newBlend = float4(blendAmount, blendAmount, blendAmount, 1.0);

    //// **過去のブレンド情報に累積**
    //BlendMap[pixelCoord] = saturate(currentBlend + newFactor);