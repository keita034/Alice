#include "Striped.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

PSOutput main(VSOutput input) : SV_TARGET
{
    PSOutput output;
    
     //テクスチャの色を得る
    float4 textureColor = tex.Sample(smp, input.uv);
    
    output.target0 = textureColor;
    
    output.target1 = float4(1 - (textureColor).rgb, 1);

    return output;
}