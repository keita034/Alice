#include "Test.hlsli"


struct Col
{
    float4 col;
};

RWStructuredBuffer<Col> col : register(u0);

VSOutput main(float4 pos : POSITION)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.svpos = pos;
    output.color = col[0].col;
    return output;
}