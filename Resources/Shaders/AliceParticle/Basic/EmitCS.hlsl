#include<BasicGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

cbuffer timeData : register(b0)
{
    float deltaTime;
    float TotalTime;
}

cbuffer emitData : register(b1)
{
    uint maxParticles;
    uint particleDataCount;
    uint emitCount;
}

cbuffer ParticleDatas : register(b2)
{
    ParticleData particleData;
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> freeList : register(u1);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= maxParticles)
       {
            return;
       }

    uint emitIndex = freeList.Consume();
    
    Particle emitParticle = ParticlePool.Load(emitIndex);

    float3 rnd = RandVec3((int) emitIndex, 1, 0);
    
    emitParticle.Position = (rnd * particleData.startPosition) - (particleData.startPosition / 2.0f);
    emitParticle.Velocity = (rnd  * particleData.velocity) - (particleData.velocity / 2.0f);
    emitParticle.Color = particleData.startColor;
    emitParticle.accel = rnd * particleData.accel;
    emitParticle.Age = 0.0f;
    emitParticle.Size = 0.5f;
    emitParticle.Alive = 1.0f;
    emitParticle.lifeTime = particleData.lifeTime;

    ParticlePool[emitIndex] = emitParticle;
}