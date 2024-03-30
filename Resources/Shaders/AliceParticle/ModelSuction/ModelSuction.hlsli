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
    
    float3 pos;
    float pad;
    
    float deltaTime;
    float lifeTime;
    float size;
    float speed;

    uint vertexSize;
    uint computeTime;
    uint maxParticles;
    uint emitSize;
};

static const uint RAND_MAX = 32767;

static const uint BONE_MAX = 128;

