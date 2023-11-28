#include "LutShader.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

Texture2D<float4> lutTex : register(t1); // 0番スロットに設定されたテクスチャ
SamplerState lutSmp : register(s1); // 0番スロットに設定されたサンプラー

float3 GetLutColor(float3 colorIN, SamplerState lutSampler)
{
    float3 color = colorIN;
    
    float2 lutSize = float2(lutWidth, lutHeight);
    float4 lutUV;
    color = saturate(color) * lutMul;
    lutUV.w = floor(color.b+ lutBlue);
    lutUV.x = (color.r + lutRed) * lutSize.x;
    lutUV.y = (color.g + lutGreen) * lutSize.y;
    lutUV.x += lutUV.w * lutSize.y;
    lutUV.z = lutUV.x + lutSize.y;
    return lerp(lutTex.SampleLevel(lutSampler, lutUV.xy, lutUV.z).rgb, lutTex.SampleLevel(lutSampler, lutUV.zy, lutUV.z).rgb, color.b - lutUV.w);
}


float4 main(VSOutput input) : SV_TARGET
{
    float4 textureColor = tex.Sample(smp, input.uv);
    
    return float4(GetLutColor(textureColor.rgb, lutSmp), 1);

}

//static float2 gamma = { 2.2, 1.0 / 2.2 };
//static float2 pi = { 3.14159256, 3.14159256 / 180.0 };
//static float2 sun = { 260, 0 };

//float4 main(VSOutput input) : SV_TARGET
//{

//    float4 color = tex.Sample(smp, input.uv);

//    color.rgb = pow(color.rgb, float3(gamma.y, gamma.y, gamma.y));

//    float u = floor(color.b * 15.0) / 15.0 * 240.0;
//    u = (floor(color.r * 15.0) / 15.0 * 15.0) + u;
//    u /= 255.0;
//    float v = 1.0 - (floor(color.g * 15.0) / 15.0);

//    float3 left0 = lutTex.Sample(lutSmp, float2(u, v)).rgb;
//    float3 left1 = lutTex.Sample(lutSmp, float2(u, v)).rgb;

//    u = ceil(color.b * 15.0) / 15.0 * 240.0;
//    u = (ceil(color.r * 15.0) / 15.0 * 15.0) + u;
//    u /= 255.0;
//    v = 1.0 - (ceil(color.g * 15.0) / 15.0);

//    float3 right0 = lutTex.Sample(lutSmp, float2(u, v)).rgb;
//    float3 right1 = lutTex.Sample(lutSmp, float2(u, v)).rgb;

//    float sunPosition = sin(sun.x * pi.y);
//    sunPosition = 0.5 * (sunPosition + 1);

//    float3 left = lerp(left0, left1, sunPosition);
//    float3 right = lerp(right0, right1, sunPosition);

//    color.r = lerp(left.r, right.r, frac(color.r * 15.0));
//    color.g = lerp(left.g, right.g, frac(color.g * 15.0));
//    color.b = lerp(left.b, right.b, frac(color.b * 15.0));

//    color.rgb = pow(color.rgb, float3(gamma.x, gamma.x, gamma.x));

//    return color;

//}