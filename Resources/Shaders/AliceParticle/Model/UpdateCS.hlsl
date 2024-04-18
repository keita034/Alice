#include<ModelGPUParticle.hlsli>

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

cbuffer BoneDatas : register(b3)
{
    matrix postureMat;
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);

StructuredBuffer<Mesh> meshs : register(t0);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= (uint) emitData.vertexSize)
    {
        return;
    }

    uint particleIndex = DTid.x;
    
    Particle particle = ParticlePool.Load(particleIndex);

    particle.position = mul(emitData.matWorld, mul(postureMat, meshs[particle.index].pos));

    ParticlePool[particleIndex] = particle;
}