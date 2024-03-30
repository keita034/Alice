#include<../GPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

cbuffer Data : register(b0)
{
    float3 centerPos;
    float speed;
    float3 accel;
    float lifeTime;
    uint maxParticles;
    uint computerTime;
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
    
    vec = normalize(vec);
    
    //particle.age = 0;
    particle.velocity = vec * speed;
    particle.accel = accel;
    
    if (lifeTime != 0)
    {
        float rnd = RandFloat(particle.index + computerTime, 1.0, 0.0);
        particle.velocity = vec * (speed * rnd);
        particle.lifeTime = lifeTime * rnd;
    }
    
    ParticlePool[DTid.x] = particle;
}