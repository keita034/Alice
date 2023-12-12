#include<../GPUParticle.hlsli>

struct EmitData
{
    float3 position;
	float maxParticles;
    
	float4 startColor;
    
	float4 endColor;
    
	float lifeTime;
    float size;
    float speed;
    uint emitCount;

    float switchingTime;
    float3 pad;
};

static const uint RAND_MAX = 32767;

static const uint EMIT_DATA_MAX = 100;
