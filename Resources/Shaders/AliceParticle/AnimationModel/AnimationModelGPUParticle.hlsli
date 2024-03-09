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

    float3 velocity;
    float maxParticles;

    float4 startColor;

    float4 endColor;

    float lifeTime;
    float size;
    float speed;
    uint vertexSize;
};

struct ModelParticle
{
    float4 color;
    float alive;

    float3 position;
    uint meshIndex;

    float3 pos;
    float size;
		
    uint4 boneIndex;
    float4 boneWeight;
};

struct SkinOutput
{
    float4 pos;
    float3 normal;
};

static const uint RAND_MAX = 32767;

static const uint EMIT_DATA_MAX = 100;

static const uint BONE_MAX = 128;

