struct VSOutput
{
    // システム用頂点座標
    float4 svpos : SV_POSITION;
    // uv値
    float4 col : COLOR;
};

struct VSInput
{
    float4 pos : POSITION;
    float4 col : COLOR;
};