#include"Model.hlsli"

// 頂点データ
static const int MAX_BONE = 128;

cbuffer SkinData : register(b3)
{
    matrix bones[MAX_BONE];
};

//スキニング計算
VSOutput ComputeSkin(VSInput input)
{
	//ゼロクリア
    VSOutput output = (VSOutput) 0;

    uint iBone; //計算するボーン番号
    float weight; //ボーンウェイト
    matrix m; //スキニング行列

	//ボーン0
    iBone = input.index.x;
    weight = input.weight.x;
    m = bones[iBone];
    output.svpos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

	//ボーン1
    iBone = input.index.y;
    weight = input.weight.y;
    m = bones[iBone];
    output.svpos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

	//ボーン2
    iBone = input.index.z;
    weight = input.weight.z;
    m = bones[iBone];
    output.svpos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

	//ボーン3
    iBone = input.index.w;
    weight = input.weight.w;
    m = bones[iBone];
    output.svpos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    return output;
}

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, float3 tangent : TANGENT, float4 color : COLOR, uint4 index : INDEX, float4 weight : WEIGHT)
{
    
    VSOutput output = (VSOutput) 0; // ピクセルシェーダーに渡す値
    uint iBone; // 計算するボーンの番号
    float boneWeight; // ボーンウェイト(重み)
    matrix m; // スキニング行列

    // ボーン0
    iBone = index.x;
    boneWeight = weight.x;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, pos);
    output.normal += boneWeight * mul((float3x3) m, normal);

    // ボーン1
    iBone = index.y;
    boneWeight = weight.y;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, pos);
    output.normal += boneWeight * mul((float3x3) m, normal);

    // ボーン2
    iBone = index.z;
    boneWeight = weight.z;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, pos);
    output.normal += boneWeight * mul((float3x3) m, normal);

    // ボーン3
    iBone = index.w;
    boneWeight = weight.w;
    m = bones[iBone];
    output.svpos += boneWeight * mul(m, pos);
    output.normal += boneWeight * mul((float3x3) m, normal);
 
    output.svpos = mul(matWorld, output.svpos);
    
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(output.normal, 0)));
    
    output.normal = wnormal.xyz;
    output.color = color;
    output.uv = uv;

    return output;
}