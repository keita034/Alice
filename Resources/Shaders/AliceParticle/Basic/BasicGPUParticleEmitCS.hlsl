#include<BasicGPUParticleEmit.hlsli>

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

	// ParticlePool�ōX�V����
    Particle emitParticle = ParticlePool.Load(emitIndex);

	//�F�ƈʒu�̓O���b�h�̈ʒu�ƃT�C�Y�Ɉˑ�����B
    emitParticle.Position = particleData.startPosition;
    emitParticle.Velocity = float3(0, 0.0f, 0.0f);
    emitParticle.Color = particleData.startColor;
    emitParticle.Age = 0.0f;
    emitParticle.Size = 0.5f;
    emitParticle.Alive = 1.0f;

	//Put it back
    ParticlePool[emitIndex] = emitParticle;
}