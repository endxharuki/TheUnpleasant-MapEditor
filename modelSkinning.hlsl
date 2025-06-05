cbuffer MatrixBuffer : register(b0)
{
    matrix ViewProjectionMatrix; // �r���[�ˉe�s��
}


StructuredBuffer<matrix> BoneMatrices : register(t0);

struct InputBoneDate
{
    float3 position : POSITION; // ���_�ʒu
    float3 normal : NORMAL; // �@��
    float2 texcoord : TEXCOORD; // �e�N�X�`�����W
    int4 boneIndices : BLENDINDICES; // �{�[���C���f�b�N�X
    float4 boneWeights : BLENDWEIGHT; // �{�[���E�F�C�g
	
};

struct OutputBoneDate
{
    float4 position : SV_POSITION; // �ϊ���̒��_�ʒu
    float3 normal : NORMAL; // �ϊ���̖@��
    float2 texcoord : TEXCOORD; // �e�N�X�`�����W
};


OutputBoneDate main(InputBoneDate date)
{
    OutputBoneDate output;

    // �X�L�j���O�s��̏�����
    matrix skinMatrix = 0;

    // �{�[���s���SRV����擾���ăX�L�j���O�s����v�Z
    for (int i = 0; i < 4; ++i)
    {
        skinMatrix += BoneMatrices[date.boneIndices[i]] * date.boneWeights[i];
    }
    
    // ���_�ʒu�Ɩ@���ɃX�L�j���O��K�p
    float4 skinnedPosition = mul(float4(date.position, 1.0f), skinMatrix);
    float4 skinnedNormal = mul(float4(date.normal, 0.0f), skinMatrix);

    // �o��
    output.position = mul(skinnedPosition, ViewProjectionMatrix); // �r���[�ˉe�s���K�p
    output.normal = normalize(skinnedNormal.xyz);
    output.texcoord = date.texcoord;

    return output;
}