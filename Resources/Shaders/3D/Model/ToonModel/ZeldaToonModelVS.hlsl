#include"ZeldaToonModel.hlsli"

cbuffer cbuff3 : register(b3)
{
    matrix postureMat;
};

VSOutput main(VSInput input)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, mul(postureMat, float4(input.normal, 0))));
    float4 wpos = mul(world, input.pos);
    
    VSOutput output = (VSOutput) 0; // �s�N�Z���V�F�[�_�[�ɓn���l
 
    output.svpos = mul(matWorld, mul(postureMat, input.pos));
    output.eyeDir = normalize(cameraPos - wpos.xyz);
    output.normal = wnormal.xyz;
    output.color = input.color;
    output.uv = input.uv;
    return output;
}