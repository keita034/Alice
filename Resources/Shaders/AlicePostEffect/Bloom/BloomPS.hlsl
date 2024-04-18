#include "Bloom.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV,pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));

}

PSOutput main(VSOutput input)
{
    PSOutput output;
    
     //テクスチャの色を得る
    float4 textureColor = tex.Sample(smp, input.uv);

    //高輝抽出
    {
        //ブラー
        {
            
            float totalWeight = 0;
            float sigma = 0.005;
            float stepWidth = 0.001;
            float4 returnColor = float4(0, 0, 0, 0);
            float4 color;
            
            for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
            {
                for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
                {
                    float2 pickUV = input.uv + float2(px, py);
                    float weight = Gaussian(input.uv, pickUV, sigma);

                    color = tex.Sample(smp, pickUV);
                    float grayScale = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
                    float extract = smoothstep(0.6, 0.9, grayScale);
                    color = color * extract;

                    returnColor += color * weight;
                    totalWeight += weight;
                }
            }

            returnColor.rgb = returnColor.rgb / totalWeight;
            returnColor.a = 1.0;

            output.target0 = returnColor;
        }
    }
    
    //そのまま
    {
        output.target1 = textureColor;
    }
   
    return output;
}