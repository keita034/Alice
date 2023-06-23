#include"Phong.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    // テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
    
    float3 ambient = texcolor.rgb * m_ambient * lightcolor.rgb;

    float intensity = saturate(dot(normalize(input.normal), lightv));
    float3 diffuse = (texcolor.rgb * m_diffuse) * intensity * lightcolor.rgb;

    float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
    input.normal = normalize(input.normal);
    float3 reflectDir = -lightv + 2 * input.normal * dot(input.normal, lightv);
    float3 specular = pow(saturate(dot(reflectDir, eyeDir)), 20) * lightcolor.rgb;

    float3 ads = ambient + diffuse + specular;

    return float4(ads, texcolor.a);
}