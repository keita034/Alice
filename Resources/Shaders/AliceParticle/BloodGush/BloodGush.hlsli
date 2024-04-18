#include<../GPUParticle.hlsli>

struct EmitData
{
    float3 position;
    float lifeTime;

    float3 velocity;
    float maxParticles;

    float3 accel;
    float speed;

    float4 startColor;

    float4 endColor;

    uint index;
    uint emitCount;
    float emitLifeTime;
    float amount;

    float2 size;
    float pad;
    float pad2;
};
