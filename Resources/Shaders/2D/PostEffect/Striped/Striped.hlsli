// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
    float4 color : COLOR; // 色(RGBA)
};

struct PSOutput
{
    float4 target0 : SV_TARGET0; // システム用頂点座標
    float4 target1 : SV_TARGET1; // システム用頂点座標
};

cbuffer cbuff0 : register(b0)
{
    float uForRange : packoffset(c0.x);
    float vForRange : packoffset(c0.y); //ワールド座標
    float blurLevel : packoffset(c0.z); //ワールド座標
    float2 windowSize : packoffset(c1); //カメラ座標(ワールド座標)
};