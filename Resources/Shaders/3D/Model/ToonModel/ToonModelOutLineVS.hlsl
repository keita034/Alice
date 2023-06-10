#include"ToonModelOutLine.hlsli"

cbuffer cbuff3 : register(b1)
{
    matrix postureMat;
};

VSOutput main(VSInput input)
{
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, mul(postureMat, float4(input.normal, 0))));
    
    VSOutput output = (VSOutput) 0; // �s�N�Z���V�F�[�_�[�ɓn���l
 
    output.svpos = mul(matWorld, mul(postureMat, input.pos + wnormal * 0.3f));
    output.color = input.color;
    return output;
}