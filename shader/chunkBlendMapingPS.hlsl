
#include "common.hlsl"
#define BLEND_MAP_RESOLUTION (512)

Texture2D baseTexture : register(t0);
Texture2D blendTexture1 : register(t1);
Texture2D<float4> BlendMap : register(t2); // CSで更新されたBlendMap
// **ポリゴンIDマップのシェーダーリソース**
Texture2D<uint> PolygonIDMapTexture : register(t3);

SamplerState samLinear : register(s0);

float4 main(PS_IN In) : SV_Target
{
    
   
    //ブレンド改良版(一つのpolygonしか無理)
    float4 baseColor = baseTexture.Sample(samLinear, In.TexCoord);
   
    uint width1, height1, mip1;
    uint width2, height2, mip2;
    baseTexture.GetDimensions(0, width1, height1, mip1);
    blendTexture1.GetDimensions(0, width2, height2, mip2);
    

    //// **UV座標を補正**
    float2 blendTexCoord = In.TexCoord * float2((float) width1 / width2, (float) height1 / height2);
    float4 blendColor = blendTexture1.Sample(samLinear, blendTexCoord);
    
    // 累積ブレンド情報を取得 (0.0 1.0 の範囲)
    float4 blendFactor = BlendMap.Sample(samLinear, In.TexCoord);

    // **ブレンド比率に応じて完全に置き換わるようにする**
    float4 finalColor = lerp(baseColor, blendColor, saturate(blendFactor.r));

    //影
    finalColor *= In.Diffuse;
    return finalColor;
    
    
    
}

 //// ポリゴンIDを取得
    //uint polygonID = PolygonIDMapTexture[In.TexCoord]; // Rチャンネルを使用
    
    //// BlendMap からブレンド情報を取得
    //float4 blendFactor = BlendMap.Sample(samLinear, In.TexCoord);
    
    //// ベーステクスチャの色を取得
    //float4 baseColor = baseTexture.Sample(samLinear, In.TexCoord);

    //// ブレンドテクスチャの色を取得
    //float4 blendColor = blendTexture1.Sample(samLinear, In.TexCoord);

    ////// ポリゴンIDによってブレンドを調整（例: IDが0ならブレンドしない）
    ////if (polygonID == 0)
    ////{
    ////    return float4(1.0f,0.0f,0.0f,1.0f); // ID=0のポリゴンはブレンドしない
    ////}
    
    //// ブレンド適用
    //float4 finalColor = lerp(baseColor, blendColor, blendFactor.r);

    //return finalColor;
    //// ブレンド計算（線形補間）
    //float4 finalColor = lerp(baseColor, blendColor, blendFactor);

    //return finalColor;
    