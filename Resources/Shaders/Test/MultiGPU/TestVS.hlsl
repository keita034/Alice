#include "Test.hlsli"


struct Col
{
    float4 col;
};

RWStructuredBuffer<Col> col : register(u0);

VSOutput main(float4 pos : POSITION)
{
    VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = pos;
    output.color = col[0].col;
    return output;
}