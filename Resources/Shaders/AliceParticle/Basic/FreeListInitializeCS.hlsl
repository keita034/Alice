#include<BasicGPUParticle.hlsli>
#include<../../HLSLMath.hlsli>

AppendStructuredBuffer<uint> freeList : register(u0);

cbuffer emitData : register(b0)
{
    uint maxParticles;
    uint particleDataCount;
}

[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= maxParticles)
    {
          return;
    }

    freeList.Append(DTid.x);
}