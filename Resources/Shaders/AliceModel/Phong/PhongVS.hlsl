#include"Phong.hlsli"

cbuffer cbuff3 : register(b3)
{
    matrix postureMat;
};

VSOutput main(VSInput input)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
    float4 wpos = mul(world, mul(postureMat, input.pos));
    
     // �s�N�Z���V�F�[�_�[�ɓn���l
    //�S�ĉ��Z����
    VSOutput output;
    output.svpos = mul(matWorld, mul(postureMat, input.pos));
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = input.uv;
    return output;
}