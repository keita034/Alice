#include"ToonModelOutLine.hlsli"

// 頂点データ
static const int MAX_BONE = 128;

cbuffer SkinData : register(b1)
{
    matrix bones[MAX_BONE];
};


VSOutput main(VSInput input)
{
    
    VSOutput output = (VSOutput) 0; // ピクセルシェーダーに渡す値
    uint iBone; // 計算するボーンの番号
    float boneWeight; // ボーンウェイト(重み)
    matrix m; // スキニング行列
    
    float3 nol = (float3)0;

    // ボーン0
    iBone = input.index.x;
    boneWeight = input.weight.x;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    nol += boneWeight * mul((float3x3) m, input.normal);

    // ボーン1
    iBone = input.index.y;
    boneWeight = input.weight.y;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    nol += boneWeight * mul((float3x3) m, input.normal);

    // ボーン2
    iBone = input.index.z;
    boneWeight = input.weight.z;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    nol += boneWeight * mul((float3x3) m, input.normal);

    // ボーン3
    iBone = input.index.w;
    boneWeight = input.weight.w;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, input.pos);
    nol += boneWeight * mul((float3x3) m, input.normal);
 
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = float4(normalize(mul((float3x3) world, nol)),0);
 
    output.svpos = mul(matWorld, output.svpos + wnormal * 0.3);
    
    output.color = input.color;

    return output;
}