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
RWStructuredBuffer<uint> DrawList : register(u2);

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
    
    float3 rand = RandomPointInUnitSphere(emitIndex + computeTime, float3(emitData.radius, emitData.radius, emitData.radius));
    
     ParticlePool[emitIndex].position = rand;
     ParticlePool[emitIndex].velocity = float3(0, 1, 0) * emitData.speed;
     ParticlePool[emitIndex].color = emitData.startColor;
     ParticlePool[emitIndex].position = rand + emitData.position;
     ParticlePool[emitIndex].age = 0.0f;
     ParticlePool[emitIndex].size = emitData.size;
     ParticlePool[emitIndex].alive = 1.0f;
     ParticlePool[emitIndex].index = emitDataIndex;
     ParticlePool[emitIndex].lifeTime = emitData.lifeTime;
}