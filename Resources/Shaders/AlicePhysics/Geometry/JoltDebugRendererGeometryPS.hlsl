#include "JoltDebugRendererGeometry.hlsli"

//Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
//SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    if (isShadow >= 0.0f)
    {
         // テクスチャマッピング
        //float4 texcolor = tex.Sample(smp, input.uv);

        float3 light_diffuse = saturate(dot(lightv, input.normal));
        float3 shade_color;
        shade_color = float3(0.4f, 0.4f, 0.4f); //アンビエント項
        shade_color += float3(0.8f, 0.8f, 0.8f) * light_diffuse; //ディフューズ項
    
        return float4(shade_color, 1.0f) * color;
    }
    else
    {
        return color;
    }

    
}