// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
    float4 color : COLOR; // 色(RGBA)
};

cbuffer cbuff0 : register(b0)
{
    float2 center;
    float power;
};