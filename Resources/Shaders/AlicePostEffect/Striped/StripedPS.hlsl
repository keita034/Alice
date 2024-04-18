#include "Striped.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

PSOutput main(VSOutput input)
{
    PSOutput output;
    
     //テクスチャの色を得る
    float4 textureColor = tex.Sample(smp, input.uv);
    
    output.target0 = textureColor;
    
    //平均ブラー
    {
        const float offsetU = blurLevel / windowSize.x;
        const float offsetV = blurLevel / windowSize.y;
        float4 color = { 0, 0, 0, 0 };
        
        for (float v = 0; v < vForRange; v++)
        {
            for (float u = 0; u < uForRange; u++)
            {
                float2 offSet = float2((u - 1) * offsetU, (v - 1) * offsetV);
                color += tex.Sample(smp, saturate(input.uv - offSet));
            }
        }
        
        output.target0 = color / (uForRange * vForRange);
        output.target0.a = 1;

    }
    
    //色反転
    {
        output.target1 = float4(1 - (textureColor).rgb, 1);
    }
    

    
    

    return output;
}