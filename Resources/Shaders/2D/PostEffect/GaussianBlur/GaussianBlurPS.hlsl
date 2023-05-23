#include "GaussianBlur.hlsli"

cbuffer cbuff1 : register(b0)
{
    float4 weight[2]; // èdÇ›
};

float4 main(VSOutput input) : SV_TARGET
{

    float4 Color = float4(0,0,0,0);

    Color += weight[0].x * sceneTexture.Sample(smp, input.tex0.xy);
    Color += weight[0].y * sceneTexture.Sample(smp, input.tex1.xy);
    Color += weight[0].z * sceneTexture.Sample(smp, input.tex2.xy);
    Color += weight[0].w * sceneTexture.Sample(smp, input.tex3.xy);
    Color += weight[1].x * sceneTexture.Sample(smp, input.tex4.xy);
    Color += weight[1].y * sceneTexture.Sample(smp, input.tex5.xy);
    Color += weight[1].z * sceneTexture.Sample(smp, input.tex6.xy);
    Color += weight[1].w * sceneTexture.Sample(smp, input.tex7.xy);

    Color += weight[0].x * sceneTexture.Sample(smp, input.tex0.zw);
    Color += weight[0].y * sceneTexture.Sample(smp, input.tex1.zw);
    Color += weight[0].z * sceneTexture.Sample(smp, input.tex2.zw);
    Color += weight[0].w * sceneTexture.Sample(smp, input.tex3.zw);
    Color += weight[1].x * sceneTexture.Sample(smp, input.tex4.zw);
    Color += weight[1].y * sceneTexture.Sample(smp, input.tex5.zw);
    Color += weight[1].z * sceneTexture.Sample(smp, input.tex6.zw);
    Color += weight[1].w * sceneTexture.Sample(smp, input.tex7.zw);

    return Color;
}