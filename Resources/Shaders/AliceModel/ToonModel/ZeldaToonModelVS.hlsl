#include"ZeldaToonModel.hlsli"

cbuffer cbuff3 : register(b3)
{
    matrix postureMat;
};

VSOutput main(VSInput input)
{
	// 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, mul(postureMat, float4(input.normal, 0))));
    float4 wpos = mul(world, input.pos);
    
    VSOutput output = (VSOutput) 0; // ピクセルシェーダーに渡す値
 
    output.svpos = mul(matWorld, mul(postureMat, input.pos));
    output.eyeDir = normalize(cameraPos - wpos.xyz);
    output.normal = wnormal.xyz;
    output.color = input.color;
    output.uv = input.uv;
    return output;
}