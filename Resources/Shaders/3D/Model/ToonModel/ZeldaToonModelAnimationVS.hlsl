#include"ZeldaToonModel.hlsli"

// 頂点データ
static const int MAX_BONE = 128;

cbuffer SkinData : register(b3)
{
    matrix bones[MAX_BONE];
};

VSOutput main(VSInput input)
{
    
    VSOutput output = (VSOutput) 0; // ピクセルシェーダーに渡す値
    uint iBone; // 計算するボーンの番号
    float boneWeight; // ボーンウェイト(重み)
    matrix m; // スキニング行列

    // ボーン0
    iBone = input.index.x;
    boneWeight = input.weight.x;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    output.normal += boneWeight * mul((float3x3) m, input.normal);

    // ボーン1
    iBone = input.index.y;
    boneWeight = input.weight.y;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    output.normal += boneWeight * mul((float3x3) m, input.normal);

    // ボーン2
    iBone = input.index.z;
    boneWeight = input.weight.z;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    output.normal += boneWeight * mul((float3x3) m, input.normal);

    // ボーン3
    iBone = input.index.w;
    boneWeight = input.weight.w;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    output.normal += boneWeight * mul((float3x3) m, input.normal);
    
    float4 wpos = mul(world, output.svpos);
    output.eyeDir = normalize(cameraPos - wpos.xyz);
    
    output.svpos = mul(matWorld, output.svpos);
    
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(output.normal, 0)));
    
    output.normal = wnormal.xyz;
    output.color = input.color;
    output.uv = input.uv;

    return output;
}