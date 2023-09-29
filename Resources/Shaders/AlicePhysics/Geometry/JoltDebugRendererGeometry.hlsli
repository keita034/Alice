struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 color : COLOR;
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // uv値
};

//3D変換行列
cbuffer ConstBufferDataTransform : register(b0)
{
    matrix world; //ワールド行列
    matrix matWorld; //ワールド座標
    float4 color;
    float isShadow ;
};

cbuffer Light : register(b1)
{
    float3 lightv : packoffset(c0); //ライトへの方向ベクトル
    float4 lightcolor : packoffset(c1); //ライトの色(RGB)
};

struct VSInput
{
    // システム用頂点座標
    float4 pos : POSITION;
    // 法線
    float3 normal : NORMAL;
    //uv値
    float2 uv : TEXCOORD;
    //カラー
    float4 col : COLOR;
};
