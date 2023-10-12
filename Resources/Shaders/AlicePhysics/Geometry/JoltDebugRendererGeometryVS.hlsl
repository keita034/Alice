#include "JoltDebugRendererGeometry.hlsli"

VSOutput main(VSInput input)
{
	// 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
    
     // ピクセルシェーダーに渡す値
    //全て加算する
    VSOutput output;
    output.svpos = mul(matWorld, float4(input.pos.xyz,1.0f));
    output.normal = wnormal.xyz;
    output.uv = input.uv;
    output.color = input.col;
    return output;
}