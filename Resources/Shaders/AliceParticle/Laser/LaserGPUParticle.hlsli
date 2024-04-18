#include<../GPUParticle.hlsli>

struct EmitData
{
    float3 velocity;
    float lifeTime;

    float3 position;
    float maxParticles;

    float4 startColor;

    float4 endColor;

    float speed;
    float maxSize;
    float maxSizeTime;
    float pad;
};


static const uint RAND_MAX = 32767;
