#include<../GPUParticle.hlsli>

struct Mesh
{
    float4 pos : POSITION; // 座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // uv座標
    float3 tangent :TANGENT; // 接空間
    float4 color :COLOR; // 頂点色
    uint4 boneIndex : BONEINDICES; // ボーンの番号
    float4 boneWeight : BONEWEIGHTS; // ボーンの重み
};

struct EmitData
{
    matrix matWorld;

    float4 startColor;

    float4 endColor;

    float deltaTime;
    float totalTime;
    uint computeTime;
    float size;

    uint vertexSize;
    uint meshIndex;
    float2 PAD;
};

struct ModelParticle
{
    float3 color;
    uint index;
    
    float3 position;
    uint alive;
    
    float3 pos;
    float size;
 
    float4 boneWeight;
    
    uint4 boneIndex;

};

struct SkinOutput
{
    float4 pos;
    float3 normal;
};

struct BoneMeshData
{
    uint sVisibles;
    float3 PAD;
};

static const uint RAND_MAX = 32767;

static const uint BONE_MAX = 128;

