#include"HlslFunc.hlsli"

struct PosNormalUv
{
    float3 pos; //座標
    float3 normal; //法線ベクトル
    float2 uv; //uv座標
};

//入力データにアクセスするための変数
StructuredBuffer<PosNormalUv> inputVer : register(t0);

//入力データにアクセスするための変数
StructuredBuffer<PosNormalUv> inputBlendVer : register(t1);

//出力データにアクセスするための変数
RWStructuredBuffer<PosNormalUv> outputVer : register(u0);

cbuffer cbuff0 : register(b0)
{
    float t;
};

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint id = DTid.x;
    outputVer[id].pos = Float3Lerp(inputBlendVer[id].pos, inputVer[id].pos, t);
    outputVer[id].normal = inputVer[id].normal;
    outputVer[id].uv = inputVer[id].uv;

}