#include<FireGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

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
ConsumeStructuredBuffer<uint> freeList : register(u1);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= maxParticles)
    {
       return;
    }

    EmitData emitData = emitDatas[emitDataIndex];
    
    if (DTid.x >= emitData.maxParticles)
    {
        return;
    }
    
    uint emitIndex = freeList.Consume();
    
    Particle emitParticle = ParticlePool.Load(emitIndex);

    float3 rand = RandomPointInUnitSphere(emitIndex + computeTime, float3(emitData.radius, emitData.radius, emitData.radius));
    
    emitParticle.position = rand;
    emitParticle.velocity = float3(0, 1, 0) * emitData.speed;
    emitParticle.color = emitData.startColor;
    emitParticle.position = rand + emitData.position;
    emitParticle.age = 0.0f;
    emitParticle.size = emitData.size;
    emitParticle.alive = 1.0f;
    emitParticle.index = emitDataIndex;
    emitParticle.lifeTime = RandFloat(emitIndex, emitData.lifeTime, emitData.lifeTime / 1000);

    ParticlePool[emitIndex] = emitParticle;
}