#include"SandStorm.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float4 main(VSOutput input) : SV_TARGET
{
    float3 textureColor = tex.Sample(smp, input.uv).rgb;
    float2 pos = input.uv;

    pos *= sin(time);
    float r = Rand(pos);
    float3 noise = float3(r, r, r);
    float noise_intensity = 0.5;
    textureColor = lerp(textureColor, noise, noise_intensity);

    return float4(textureColor, 1.0);
}