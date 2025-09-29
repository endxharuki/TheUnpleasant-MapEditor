
#include "common.hlsl"
#define BLEND_MAP_RESOLUTION (512)

Texture2D baseTexture : register(t0);
Texture2D blendTexture1 : register(t1);
Texture2D<float4> BlendMap : register(t2); // CS�ōX�V���ꂽBlendMap
// **�|���S��ID�}�b�v�̃V�F�[�_�[���\�[�X**
Texture2D<uint> PolygonIDMapTexture : register(t3);

SamplerState samLinear : register(s0);

float4 main(PS_IN In) : SV_Target
{
    
   
    //�u�����h���ǔ�(���polygon��������)
    float4 baseColor = baseTexture.Sample(samLinear, In.TexCoord);
   
    uint width1, height1, mip1;
    uint width2, height2, mip2;
    baseTexture.GetDimensions(0, width1, height1, mip1);
    blendTexture1.GetDimensions(0, width2, height2, mip2);
    

    //// **UV���W��␳**
    float2 blendTexCoord = In.TexCoord * float2((float) width1 / width2, (float) height1 / height2);
    float4 blendColor = blendTexture1.Sample(samLinear, blendTexCoord);
    
    // �ݐσu�����h�����擾 (0.0 1.0 �͈̔�)
    float4 blendFactor = BlendMap.Sample(samLinear, In.TexCoord);

    // **�u�����h�䗦�ɉ����Ċ��S�ɒu�������悤�ɂ���**
    float4 finalColor = lerp(baseColor, blendColor, saturate(blendFactor.r));

    //�e
    finalColor *= In.Diffuse;
    return finalColor;
    
    
    
}

 //// �|���S��ID���擾
    //uint polygonID = PolygonIDMapTexture[In.TexCoord]; // R�`�����l�����g�p
    
    //// BlendMap ����u�����h�����擾
    //float4 blendFactor = BlendMap.Sample(samLinear, In.TexCoord);
    
    //// �x�[�X�e�N�X�`���̐F���擾
    //float4 baseColor = baseTexture.Sample(samLinear, In.TexCoord);

    //// �u�����h�e�N�X�`���̐F���擾
    //float4 blendColor = blendTexture1.Sample(samLinear, In.TexCoord);

    ////// �|���S��ID�ɂ���ău�����h�𒲐��i��: ID��0�Ȃ�u�����h���Ȃ��j
    ////if (polygonID == 0)
    ////{
    ////    return float4(1.0f,0.0f,0.0f,1.0f); // ID=0�̃|���S���̓u�����h���Ȃ�
    ////}
    
    //// �u�����h�K�p
    //float4 finalColor = lerp(baseColor, blendColor, blendFactor.r);

    //return finalColor;
    //// �u�����h�v�Z�i���`��ԁj
    //float4 finalColor = lerp(baseColor, blendColor, blendFactor);

    //return finalColor;
    