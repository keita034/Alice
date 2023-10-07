#include<BasicGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

cbuffer objectData : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer timeData : register(b1)
{
    float deltaTime;
    float TotalTime;
}

cbuffer emitData : register(b2)
{
    uint emitCount;
    float particleDataCount;
}

RWStructuredBuffer<ParticleData> particleDatas : register(u0);
RWStructuredBuffer<Particle> ParticlePool : register(u1);
ConsumeStructuredBuffer<uint> freeList : register(u2);
RWStructuredBuffer<uint> DrawList : register(u3);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= (uint) emitCount)
        return;

    uint emitIndex = freeList.Consume();
    
    ParticleData particleData = particleDatas[emitIndex % particleDataCount];

	// ParticlePoolで更新する
    Particle emitParticle = ParticlePool.Load(emitIndex);

    float3 rnd = RandVec3((int) TotalTime, RAND_MAX, 0);
    
	//色と位置はグリッドの位置とサイズに依存する。
    emitParticle.Position = rnd / RAND_MAX * particleData.startPosition - particleData.startPosition / 2.0f;
    emitParticle.Velocity = rnd / RAND_MAX * particleData.velocity - particleData.velocity / 2.0f;
    emitParticle.Color = particleData.startColor;
    emitParticle.accel = rnd / RAND_MAX * particleData.accel;
    emitParticle.Age = 0.0f;
    emitParticle.Size = 0.5f;
    emitParticle.Alive = 1.0f;
    emitParticle.index = emitIndex % particleDataCount;

	//Put it back
    ParticlePool[emitIndex] = emitParticle;
}