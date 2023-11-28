#include"ZeldaToonModel.hlsli"

// ���_�f�[�^
static const int MAX_BONE = 128;

cbuffer SkinData : register(b3)
{
    matrix bones[MAX_BONE];
};

VSOutput main(VSInput input)
{
    
    VSOutput output = (VSOutput) 0; // �s�N�Z���V�F�[�_�[�ɓn���l
    uint iBone; // �v�Z����{�[���̔ԍ�
    float boneWeight; // �{�[���E�F�C�g(�d��)
    matrix m; // �X�L�j���O�s��

    // �{�[��0
    iBone = input.index.x;
    boneWeight = input.weight.x;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    output.normal += boneWeight * mul((float3x3) m, input.normal);

    // �{�[��1
    iBone = input.index.y;
    boneWeight = input.weight.y;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    output.normal += boneWeight * mul((float3x3) m, input.normal);

    // �{�[��2
    iBone = input.index.z;
    boneWeight = input.weight.z;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    output.normal += boneWeight * mul((float3x3) m, input.normal);

    // �{�[��3
    iBone = input.index.w;
    boneWeight = input.weight.w;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    output.normal += boneWeight * mul((float3x3) m, input.normal);
    
    float4 wpos = mul(world, output.svpos);
    output.eyeDir = normalize(cameraPos - wpos.xyz);
    
    output.svpos = mul(matWorld, output.svpos);
    
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(output.normal, 0)));
    
    output.normal = wnormal.xyz;
    output.color = input.color;
    output.uv = input.uv;

    return output;
}