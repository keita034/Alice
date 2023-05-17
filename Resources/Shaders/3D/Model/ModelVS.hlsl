#include"ModelSimple.hlsli"

cbuffer cbuff3 : register(b3)
{
    matrix postureMat;
};

VSOutput main(VSInput input)
{
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
    
    VSOutput output = (VSOutput) 0; // ピクセルシェーダーに渡す値
 
    output.svpos = mul(matWorld, mul(postureMat, input.pos));
    output.svpos.z = 0;
    output.normal = wnormal.xyz;
    output.color = input.color;
    output.uv = input.uv;
    return output;
}