#include"ToonModel.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー
Texture2D<float4> ramp : register(t1); //1番スロットに設定されたテクスチャ
SamplerState rampSmp : register(s1); //1番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    //ハーフランバート
    float light_diffuse = (dot(input.normal, lightv));
    light_diffuse = light_diffuse * 0.5f + 0.5f;
    light_diffuse = light_diffuse * light_diffuse;
    
    float4 texcolor = tex.Sample(smp, input.uv);
 
    float4 rampCol = ramp.Sample(rampSmp, float2(light_diffuse, 0.5));
 
    return float4(texcolor.rgb * rampCol.rgb, texcolor.a * m_alpha);
}