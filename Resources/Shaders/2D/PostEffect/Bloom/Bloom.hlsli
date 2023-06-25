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