#include<ModelGPUParticle.hlsli>
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
    EmitData emitData;
}

cbuffer BoneDatas : register(b3)
{
    matrix postureMat;
}

StructuredBuffer<Mesh> meshs : register(t0);

RWStructuredBuffer<Particle> ParticlePool : register(u0);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= maxParticles)
    {
       return;
    }

    if (DTid.x >= emitData.vertexSize)
    {
        return;
    }
    
    uint emitIndex = DTid.x;

    ParticlePool[emitIndex].position = mul(emitData.matWorld, mul(postureMat, meshs[DTid.x].pos));
    ParticlePool[emitIndex].velocity = float3(0,0,0);
    ParticlePool[emitIndex].color = emitData.startColor;
    ParticlePool[emitIndex].age = 0;
    ParticlePool[emitIndex].size = float2(emitData.size, emitData.size);
    ParticlePool[emitIndex].alive = 1.0f;
    ParticlePool[emitIndex].index = DTid.x;
    ParticlePool[emitIndex].lifeTime = emitData.lifeTime;
}