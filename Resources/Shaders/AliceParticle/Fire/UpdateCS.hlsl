#include<FireGPUParticle.hlsli>

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
    EmitData emitDatas[EMIT_DATA_MAX];
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> freeList : register(u1);
RWStructuredBuffer<uint> DrawList : register(u2);

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
    
    EmitData emitData = emitDatas[emitDataIndex];
    
    float t = particle.age / emitData.lifeTime;
    
    particle.age += deltaTime;

    particle.alive = (float) (particle.age < particle.lifeTime);

    particle.position += particle.velocity * deltaTime;
    
    particle.color = lerp(emitData.startColor, emitData.endColor, t);
    
    particle.threshold = t;

    ParticlePool[DTid.x] = particle;

    if (particle.alive == 0.0f)
    {
        freeList.Append(DTid.x);
    }
    else
    {
        uint drawIndex = DrawList.IncrementCounter();
        uint drawData;
        drawData = DTid.x;

        DrawList[drawIndex] = drawData;
    }
}