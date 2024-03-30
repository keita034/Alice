//struct IndirectCommand
//{
//    uint vertexCountPerInstance;
//    uint instanceCount;
//    uint startVertexLocation;
//    uint startInstanceLocation;
//};

//struct VS_OUTPUT
//{
//    float3 position : POSITION;
//    float4 color : COLOR;
//    float2 size : SIZE;
//    float threshold : THRESHOLD;
//};

//struct GS_OUTPUT
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//    float2 uv : TEXCOORD;
//    float threshold : THRESHOLD;
//};

////四角形の頂点
//static const uint vnum = 4;

////センターからオフセット
//static const float4 offset_array[vnum] =
//{
//    float4(-0.5f, -0.5f, 0, 0), //左下
//    float4(-0.5f, +0.5f, 0, 0), //左上
//    float4(+0.5f, -0.5f, 0, 0), //右下
//    float4(+0.5f, +0.5f, 0, 0) //右上
//};

////左上が0,0 右下が1,1

//static const float2 uv_array[vnum] =
//{
//    float2(0, 1), //左下
//    float2(0, 0), //左上
//    float2(1, 1), //右下
//    float2(1, 0), //右上
//};

//struct ModelParticle
//{
//    float4 color;

//    float3 position;
//    float age;

//    float3 movePos;
//    float alive;

//    float3 meshPos;
//    float invTime;
    
//    float moveTime;
//    float totalTime;
//    float2 pad;
    
//    float2 size;
//    float threshold;
//    float lifeTime;
//};

//struct Mesh
//{
//    float4 pos : POSITION; // 座標
//    float3 normal : NORMAL; // 法線
//    float2 uv : TEXCOORD; // uv座標
//    float3 tangent :TANGENT; // 接空間
//    float4 color :COLOR; // 頂点色
//    uint4 boneIndex : BONEINDICES; // ボーンの番号
//    float4 boneWeight : BONEWEIGHTS; // ボーンの重み
//};

//struct EmitData
//{
//    matrix matWorld;

//    float4 startColor;

//    float4 endColor;

//    float3 centerPos;
//    float distance;

//    uint computeTime;
//    float deltaTime;
//    float lifeTime;
//    float invisibleTime;

//    float moveTime;
//    float size;
//    uint vertexSize;
//    uint maxParticles;
//};

//static const uint RAND_MAX = 32767;

//static const uint BONE_MAX = 128;

#include<../GPUParticle.hlsli>

struct Mesh
{
    float4 pos : POSITION; // 座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // uv座標
    float3 tangent : TANGENT; // 接空間
    float4 color : COLOR; // 頂点色
    uint4 boneIndex : BONEINDICES; // ボーンの番号
    float4 boneWeight : BONEWEIGHTS; // ボーンの重み
};

struct EmitData
{
    matrix matWorld;

    float4 startColor;

    float4 endColor;

    float3 centerPos;
    float distance;

    float lifeTime;
    float invisibleTime;
    float moveTime;
    float size;
    
    uint vertexSize;
    uint maxParticles;
    float2 pad;
};

static const uint RAND_MAX = 32767;

static const uint BONE_MAX = 128;

struct ModelParticle
{
    float4 color;

    float3 position;
    float age;

    float3 movePos;
    float alive;

    float3 meshPos;
    float invTime;
    
    float moveTime;
    float totalTime;
    float2 pad;
    
    float2 size;
    float threshold;
    float lifeTime;
};