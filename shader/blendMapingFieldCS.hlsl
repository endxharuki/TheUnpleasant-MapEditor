#include "common.hlsl"

#define BLEND_MAP_RESOLUTION (512)
#define FIELD_X (19.0)
#define FIELD_Z (19.0)


RWTexture2D<float4> FieldBlendMap : register(u0);//�ꖇ�̃u�����h�ɂ����o�[�W����
RWTexture2D<uint> PolygonIDMap : register(u1); // �e�s�N�Z����������|���S��ID


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
[numthreads(20, 20, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    
    //�ꖇ�̃e�N�X�`����
    float2 fieldUV = DTid.xy / float2(BLEND_MAP_RESOLUTION * FIELD_X, BLEND_MAP_RESOLUTION * FIELD_Z);
    
    float x = hitPolygonIndex.x;
    float y = hitPolygonIndex.y;
    
    float uvX = (x / FIELD_X);
    float uvY = (y / FIELD_Z);
    
    float addX = hitUV.x / FIELD_X;
    float addY = hitUV.y / FIELD_Z;
    
    float2 FieldUV = (addX + uvX,addY + uvY);
    
    //// ���݂̃u�����h�l���擾
    float4 fieldCurrentFactor = FieldBlendMap[DTid.xy];

    // �u�����h�̑����� (Ray �̉e���͈͓��Ȃ瑝��)
    float fieldDistance = length(fieldUV - hitUV);
    float fieldIncrease = (fieldDistance < brushRadius) ? 0.02 : 0.0; // ���X�ɑ��₷

    // **�ߋ��̒l�����Z���ėݐ�**
    float4 FieldNewFactor = saturate(fieldCurrentFactor + float4(fieldIncrease, fieldIncrease, fieldIncrease, 0));

    // �X�V
    FieldBlendMap[DTid.xy] = FieldNewFactor;
    
    
}