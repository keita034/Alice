#include"OilPainting.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float2 uv = input.uv;
 
    float3 mean[4] =
    {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };
 
    float3 sigma[4] =
    {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };
 
    float2 start[4] = { { -radius, -radius }, { -radius, 0 }, { 0, -radius }, { 0, 0 } };
 
    float2 pos;
    float3 col;
    for (int k = 0; k < 4; k++)     
    {
        for (int i = 0; i <= radius; i++)
        { 
            for (int j = 0; j <= radius; j++)
            {
                pos = float2(i, j) + start[k];
                col = tex.SampleLevel(smp, uv + float2(pos.x * TexSize.x, pos.y * TexSize.y), 0).rgb;
                mean[k] += col;
                sigma[k] += col * col;
                }
        }
    }
 
    float sigma2;
 
    float n = pow(radius + 1, 2);
    float4 color = tex.Sample(smp, uv);
    float min = 1;
 
    for (int l = 0; l < 4; l++)
    {
        mean[l] /= n;
        sigma[l] = abs(sigma[l] / n - mean[l] * mean[l]);
        sigma2 = sigma[l].r + sigma[l].g + sigma[l].b;
 
        if (sigma2 < min)
        {
            min = sigma2;
            color.rgb = mean[l].rgb;
        }
    }

    return color;
}