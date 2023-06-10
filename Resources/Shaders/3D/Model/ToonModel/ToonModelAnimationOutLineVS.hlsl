#include"ToonModelOutLine.hlsli"

// ���_�f�[�^
static const int MAX_BONE = 128;

cbuffer SkinData : register(b1)
{
    matrix bones[MAX_BONE];
};


VSOutput main(VSInput input)
{
    
    VSOutput output = (VSOutput) 0; // �s�N�Z���V�F�[�_�[�ɓn���l
    uint iBone; // �v�Z����{�[���̔ԍ�
    float boneWeight; // �{�[���E�F�C�g(�d��)
    matrix m; // �X�L�j���O�s��
    
    float3 nol = (float3)0;

    // �{�[��0
    iBone = input.index.x;
    boneWeight = input.weight.x;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    nol += boneWeight * mul((float3x3) m, input.normal);

    // �{�[��1
    iBone = input.index.y;
    boneWeight = input.weight.y;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    nol += boneWeight * mul((float3x3) m, input.normal);

    // �{�[��2
    iBone = input.index.z;
    boneWeight = input.weight.z;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    nol += boneWeight * mul((float3x3) m, input.normal);

    // �{�[��3
    iBone = input.index.w;
    boneWeight = input.weight.w;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    nol += boneWeight * mul((float3x3) m, input.normal);
 
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = float4(normalize(mul((float3x3) world, nol)),0);
 
    output.svpos = mul(matWorld, output.svpos + wnormal * 0.3);
    
    output.color = input.color;

    return output;
}