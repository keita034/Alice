#include"Monochrome.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float3 textureColor = tex.Sample(smp, input.uv).rgb;

    textureColor = dot(textureColor, float3(0.4f, 0.4f, 0.4f));
    return float4(textureColor, 1.0);
    
}