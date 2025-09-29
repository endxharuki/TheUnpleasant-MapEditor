#include "common.hlsl"

#define BLEND_MAP_RESOLUTION (512)
#define FIELD_X (19.0)
#define FIELD_Z (19.0)

// �u�����h���̍X�V���s��Compute Shader
RWTexture2D<float4> BlendMap : register(u0); // �u�����h�����i�[����e�N�X�`��
RWTexture2D<float4> FieldBlendMap : register(u1);//�ꖇ�̃u�����h�ɂ����o�[�W����
RWTexture2D<uint> PolygonIDMap : register(u2); // �e�s�N�Z����������|���S��ID


// C++ �����瑗����o�b�t�@
cbuffer PolygonDataBuffer : register(b0)
{
    float2 hitUV; // Ray����������UV���W
    float brushRadius; // �u���V�̉e���͈�
    int hitPolygonID; // 16�o�C�g�A���C�����g�̂��߂̃p�f�B���O
    int2 hitPolygonIndex; //polygon��x��z�̔ԍ�
    int polygonCount; // �|���S����
    float padding; // 12�o�C�g (16�o�C�g�A���C�����g�m��)
}

// �X���b�h���Ƃ̏���
[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    
    //�u�����h�}�b�s���O���ꉞ�ł��Ă�
    float2 uv = DTid.xy / float2(BLEND_MAP_RESOLUTION, BLEND_MAP_RESOLUTION);

    //// ���݂̃u�����h�l���擾
    float4 currentFactor = BlendMap[DTid.xy];

    // �u�����h�̑����� (Ray �̉e���͈͓��Ȃ瑝��)
    float distance = length(uv - hitUV);
    float increase = (distance < brushRadius) ? 0.02 : 0.0; // ���X�ɑ��₷

    // **�ߋ��̒l�����Z���ėݐ�**
    float4 newFactor = saturate(currentFactor + float4(increase, increase, increase, 0));

    // �X�V
    BlendMap[DTid.xy] = newFactor;
    
    
}

////���s
    //uint2 pixelCoord = DTid.xy;
    //float2 uv = pixelCoord / float2(BLEND_MAP_RESOLUTION, BLEND_MAP_RESOLUTION);

    //// ���݂̃|���S�� ID ���擾
    //uint polygonID = PolygonIDMap[pixelCoord];

    //// �q�b�g�����|���S���ƈ�v���Ȃ��ꍇ�͉������Ȃ�
    //if (polygonID != hitPolygonID)
    //{
    //    //BlendMap[pixelCoord] = float4(0, 0, 1, 1);
    //    return;
    //}

    //// �u�����h�̑����� (Ray �̉e���͈͓��Ȃ瑝��)
    //float distance = length(uv - hitUV);
    //float increase = (distance < brushRadius) ? 0.02 : 0.0; // ���X�ɑ��₷
   
    //// **���݂̃u�����h�l���擾**
    //float4 currentBlend = BlendMap[pixelCoord];

    ////// **�u�����h���x���v�Z�i���S�قǋ����u�����h�j**
    ////float blendStrength = 1.0 - saturate(distance / brushRadius);
    ////float blendAmount = 0.05 * blendStrength; // �������u�����h
    
    //// **�ߋ��̒l�����Z���ėݐ�**
    //float4 newFactor = saturate(currentBlend + float4(increase, increase, increase, 0));

    ////// **�V�����u�����h�l�����Z**
    ////float4 newBlend = float4(blendAmount, blendAmount, blendAmount, 1.0);

    //// **�ߋ��̃u�����h���ɗݐ�**
    //BlendMap[pixelCoord] = saturate(currentBlend + newFactor);