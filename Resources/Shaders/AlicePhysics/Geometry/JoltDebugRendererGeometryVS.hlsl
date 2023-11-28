#include "JoltDebugRendererGeometry.hlsli"

VSOutput main(VSInput input)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
    
     // �s�N�Z���V�F�[�_�[�ɓn���l
    //�S�ĉ��Z����
    VSOutput output;
    output.svpos = mul(matWorld, float4(input.pos.xyz,1.0f));
    output.normal = wnormal.xyz;
    output.uv = input.uv;
    output.color = input.col;
    return output;
}