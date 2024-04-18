cbuffer cbuff0 : register(b0)
{
    matrix world; //ワールド行列
    matrix matWorld; //ワールド座標
    float3 cameraPos; //カメラ座標(ワールド座標)
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 color : COLOR;
};

struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
    float4 color : COLOR;
    uint4 index : INDEX;
    float4 weight : WEIGHT;
};