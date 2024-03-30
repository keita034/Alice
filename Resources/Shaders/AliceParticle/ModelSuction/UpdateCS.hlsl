#include<ModelSuction.hlsli>


cbuffer ParticleDatas : register(b0)
{
    EmitData emitData;
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> freeList : register(u1);
AppendStructuredBuffer<uint> DrawList : register(u2);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= (uint) emitData.maxParticles)
    {
        return;
    }

    Particle particle = ParticlePool.Load(DTid.x);

    if (particle.alive == 0.0f)
    {
        return;
    }

    float t = particle.age / emitData.lifeTime;
    
    particle.age += emitData.deltaTime;

    particle.alive = (float) (particle.age < particle.lifeTime );

    particle.position = lerp(particle.accel, emitData.pos, t);
    
    particle.color = lerp(emitData.startColor, emitData.endColor, t);
    
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