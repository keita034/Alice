#include<BloodGush.hlsli>
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

    uint emitIndex = freeList.Consume();
    
    float rand = RandFloat(emitIndex + DTid.x, 1.0f, 0.0f);
    
    float rand2 = RandFloat(emitIndex + computeTime + DTid.x, 1.0f, -0.2f);

    ParticlePool[emitIndex].position = emitData.position;
    ParticlePool[emitIndex].velocity = emitData.velocity * (emitData.speed * rand);
    ParticlePool[emitIndex].color = emitData.startColor;
    ParticlePool[emitIndex].age = 0;
    ParticlePool[emitIndex].size = emitData.size;
    ParticlePool[emitIndex].alive = 1.0f;
    ParticlePool[emitIndex].index = emitDataIndex;
    ParticlePool[emitIndex].lifeTime = emitData.lifeTime * rand;
    ParticlePool[emitIndex].accel = emitData.accel * rand2;

}