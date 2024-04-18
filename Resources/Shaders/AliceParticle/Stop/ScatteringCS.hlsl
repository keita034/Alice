#include<../GPUParticle.hlsli>

cbuffer Data : register(b0)
{
    float3 centerPos;
    float radius;
    uint maxParticles;
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= (uint) maxParticles)
    {
        return;
    }

    Particle particle = ParticlePool.Load(DTid.x);

    if (particle.alive == 0.0f)
    {
        return;
    }
    
    float3 vec = particle.position - centerPos;
    
    float l = length(vec);
    
    if (l<= radius)
    {
        particle.velocity = float3(0, 0, 0);
    }

    ParticlePool[DTid.x] = particle;
}