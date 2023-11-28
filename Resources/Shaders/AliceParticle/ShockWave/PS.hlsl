#include<ShockWaveGPUParticle.hlsli>

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(GS_OUTPUT input) : SV_TARGET
{
    float4 texCol = tex.Sample(smp, input.uv);
   
    float4 col = input.color * step(input.threshold + 0.0001f, texCol.r);
  
    return col;
}