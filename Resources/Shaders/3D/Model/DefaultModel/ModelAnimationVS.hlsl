#include"Model.hlsli"

// ���_�f�[�^
static const int MAX_BONE = 128;

cbuffer SkinData : register(b3)
{
    matrix bones[MAX_BONE];
};

//�X�L�j���O�v�Z
VSOutput ComputeSkin(VSInput input)
{
	//�[���N���A
    VSOutput output = (VSOutput) 0;

    uint iBone; //�v�Z����{�[���ԍ�
    float weight; //�{�[���E�F�C�g
    matrix m; //�X�L�j���O�s��

	//�{�[��0
    iBone = input.index.x;
    weight = input.weight.x;
    m = bones[iBone];
    output.svpos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

	//�{�[��1
    iBone = input.index.y;
    weight = input.weight.y;
    m = bones[iBone];
    output.svpos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

	//�{�[��2
    iBone = input.index.z;
    weight = input.weight.z;
    m = bones[iBone];
    output.svpos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

	//�{�[��3
    iBone = input.index.w;
    weight = input.weight.w;
    m = bones[iBone];
    output.svpos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    return output;
}

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, float3 tangent : TANGENT, float4 color : COLOR, uint4 index : INDEX, float4 weight : WEIGHT)
{
    
    VSOutput output = (VSOutput) 0; // �s�N�Z���V�F�[�_�[�ɓn���l
    uint iBone; // �v�Z����{�[���̔ԍ�
    float boneWeight; // �{�[���E�F�C�g(�d��)
    matrix m; // �X�L�j���O�s��

    // �{�[��0
    iBone = index.x;
    boneWeight = weight.x;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, pos);
    output.normal += boneWeight * mul((float3x3) m, normal);

    // �{�[��1
    iBone = index.y;
    boneWeight = weight.y;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, pos);
    output.normal += boneWeight * mul((float3x3) m, normal);

    // �{�[��2
    iBone = index.z;
    boneWeight = weight.z;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, pos);
    output.normal += boneWeight * mul((float3x3) m, normal);

    // �{�[��3
    iBone = index.w;
    boneWeight = weight.w;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, pos);
    output.normal += boneWeight * mul((float3x3) m, normal);
 
    output.svpos = mul(matWorld, output.svpos);
    
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(output.normal, 0)));
    
    output.normal = wnormal.xyz;
    output.color = color;
    output.uv = uv;

    return output;
}