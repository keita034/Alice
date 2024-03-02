#include<ShockWaveGPUParticle.hlsli>

cbuffer timeData : register(b0)
{
    float deltaTime;
    float TotalTime;
    uint computeTime;
}

cbuffer ParticleData : register(b1)
{
    uint maxParticles;
    uint emitDataIndex;
}

cbuffer ParticleDatas : register(b2)
{
    EmitData emitData;
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> freeList : register(u1);
AppendStructuredBuffer<uint> DrawList : register(u2);

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
    
    float t = particle.age / emitData.lifeTime;
    
    particle.age += deltaTime;

    particle.alive = (float) (particle.age < particle.lifeTime);

    particle.position += particle.velocity * deltaTime;
    
    if(emitData.switchingTime <= particle.age)
    {
         particle.color = emitData.endColor;
    }
    
    particle.threshold = t;

    ParticlePool[DTid.x] = particle;

    if (particle.alive == 0.0f)
    {
        freeList.Append(DTid.x);
    }
    else
    {

        DrawList.Append(DTid.x);
    }
}