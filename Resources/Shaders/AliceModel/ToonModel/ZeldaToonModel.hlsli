cbuffer cbuff0 : register(b0)
{
    matrix world; //ワールド行列
    matrix matWorld; //ワールド座標
    float3 cameraPos; //カメラ座標(ワールド座標)
};

cbuffer material : register(b1)
{
    float3 m_ambient : packoffset(c0); //アンビエント係数
    float3 m_diffuse : packoffset(c1); //ディフューズ係数
    float3 m_specular : packoffset(c2); //スペキュラー係数
    float3 emission : packoffset(c3); //エミッション係数
    float shininess : packoffset(c4.x); //光沢
    float m_alpha : packoffset(c4.y); //アルファ
};

cbuffer cbuff2 : register(b2)
{
    float3 lightv : packoffset(c0); //ライトへの方向ベクトル
    float4 lightcolor : packoffset(c1); //ライトの色(RGB)
};



// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float3 eyeDir : EYEDIR;
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // uv値
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