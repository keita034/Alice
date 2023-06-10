#include"ToonModelOutLine.hlsli"

cbuffer cbuff3 : register(b1)
{
    matrix postureMat;
};

VSOutput main(VSInput input)
{
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, mul(postureMat, float4(input.normal, 0))));
    
    VSOutput output = (VSOutput) 0; // ピクセルシェーダーに渡す値
 
    output.svpos = mul(matWorld, mul(postureMat, input.pos + wnormal * 0.3f));
    output.color = input.color;
    return output;
}