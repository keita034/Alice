#include"FrostedGlass.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float4 main(VSOutput input) : SV_TARGET
{
    float x = (input.uv.x * intensity.x) + Rand(input.uv) * radius * 2.0 - radius;
    float y = (input.uv.y * intensity.y) + Rand(float2(input.uv.y, input.uv.x)) * radius * 2.0 - radius;
 
    return tex.Sample(smp, float2(x / intensity.x, y / intensity.y));
}