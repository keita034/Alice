#include"Model.hlsli"

cbuffer cbuff3 : register(b3)
{
    matrix postureMat;
};

VSOutput main(VSInput input)
{
	// 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, mul(postureMat, float4(input.normal, 0))));
    float4 wpos = mul(world, input.pos);
    
     // ピクセルシェーダーに渡す値
    //全て加算する
    VSOutput output;
    output.svpos =mul(matWorld, mul(postureMat, input.pos));
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}