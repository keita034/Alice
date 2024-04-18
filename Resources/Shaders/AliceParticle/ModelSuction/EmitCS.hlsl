#include<ModelSuction.hlsli>
#include<../../HLSLMath.hlsli>

cbuffer ParticleDatas : register(b0)
{
    EmitData emitData;
}

cbuffer BoneDatas : register(b1)
{
    matrix postureMat;
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> freeList : register(u1);

StructuredBuffer<Mesh> meshs : register(t0);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= emitData.maxParticles)
    {
       return;
    }
    
    if (DTid.x >= emitData.emitSize)
    {
        return;
    }
    
    uint emitIndex = freeList.Consume();
    uint meshIndex = emitData.vertexSize * RandFloat(emitIndex + emitData.computeTime, 1.0, 0.0);
    
    ParticlePool[emitIndex].position = mul(emitData.matWorld, mul(postureMat, meshs[meshIndex].pos));
    ParticlePool[emitIndex].velocity = float3(emitData.matWorld[3][0], emitData.matWorld[3][1], emitData.matWorld[3][2]);
    ParticlePool[emitIndex].color = emitData.startColor;
    ParticlePool[emitIndex].age = 0;
    ParticlePool[emitIndex].size = float2(emitData.size, emitData.size);
    ParticlePool[emitIndex].alive = 1.0f;
    ParticlePool[emitIndex].index = meshIndex;
    ParticlePool[emitIndex].lifeTime = emitData.lifeTime;
    ParticlePool[emitIndex].accel = mul(emitData.matWorld, mul(postureMat, meshs[meshIndex].pos));

}