#include<MeshGPUParticle.hlsli>
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

cbuffer BoneDatas : register(b3)
{
    matrix postureMat;
}

RWStructuredBuffer<Particle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> freeList : register(u1);

//Texture2D<float4> tex : register(t0);
StructuredBuffer<Mesh> meshs : register(t0);
StructuredBuffer<uint> indices : register(t1);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= maxParticles)
    {
       return;
    }
    
    //int2 uv;
    //float2 texSize;
    //tex.GetDimensions(texSize.x, texSize.y);
    
    //uv = int2(texSize * meshs[DTid.x].uv);
    
    //float4 texcolor = tex[uv];
    
    //if (texcolor.a == 0.0f)
    //{
    //    return;
    //}
    
    EmitData emitData = emitDatas[emitDataIndex];

    if (DTid.x >= emitData.vertexSize)
    {
        return;
    }
    
    uint emitIndex = freeList.Consume();

    ParticlePool[emitIndex].position = mul(emitData.matWorld, mul(postureMat, meshs[DTid.x].pos));
    ParticlePool[emitIndex].velocity = emitData.velocity * emitData.speed;
    ParticlePool[emitIndex].color = emitData.startColor;
    ParticlePool[emitIndex].age = 0;
    ParticlePool[emitIndex].size = float2(emitData.size, emitData.size);
    ParticlePool[emitIndex].alive = 1.0f;
    ParticlePool[emitIndex].index = 0;
    ParticlePool[emitIndex].lifeTime = emitData.lifeTime;
    
}