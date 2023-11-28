#include<LaserGPUParticle.hlsli>

Texture2D<float4> mainTex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState mainSmp : register(s0); // 0番スロットに設定されたサンプラー

Texture2D<float4> subTex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState subSmp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(GS_OUTPUT input) : SV_TARGET
{
    float4 texCol = mainTex.Sample(mainSmp, input.uv);
   
    float3 col = input.color.rgb * texCol.rgb;
    float a = input.color.a * texCol.a;
  
    return float4(col,a);
}