#include<BasicGPUParticle.hlsli>

float4 main(GS_OUTPUT input) : SV_TARGET
{
    input.uv = input.uv * 2 - 1;

    float fade = saturate(distance(float2(0, 0), input.uv));
    float3 color = lerp(input.color.rgb, float3(0, 0, 0), fade * fade);

    return float4(color, 1);
}