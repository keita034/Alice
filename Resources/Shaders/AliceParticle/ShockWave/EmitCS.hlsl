#include<ShockWaveGPUParticle.hlsli>
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
    
    if (DTid.x >= emitData.emitCount)
    {
        return;
    }
    
    uint emitIndex = freeList.Consume();
    
    Particle emitParticle = ParticlePool.Load(emitIndex);

    float lAngleDiv = PI2 / emitData.emitCount;
    
    float3 rand;
    rand.x = cos(lAngleDiv * DTid.x );
    rand.y = 0.0f;
    rand.z = sin(lAngleDiv * DTid.x );

    emitParticle.position = emitData.position;
    emitParticle.velocity = rand* emitData.speed;
    emitParticle.color = emitData.startColor;
    emitParticle.position = rand + emitData.position;
    emitParticle.age = 0.0f;
    emitParticle.size = float2(emitData.size, emitData.size);
    emitParticle.alive = 1.0f;
    emitParticle.index = emitDataIndex;
    emitParticle.lifeTime = emitData.lifeTime;

    ParticlePool[emitIndex] = emitParticle;
}